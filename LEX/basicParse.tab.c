/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.2"

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


//#define YYDEBUG 1

	#ifdef __cplusplus
		extern "C" {
	#endif

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
        #include <math.h>
        #include <errno.h>
        #include "../BasicTypes.h"
	#include "../Constants.h"
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
        extern int filenumber;
        extern char* include_filenames[];
        extern int include_filenames_counter;

	int *wordCode = NULL;
	unsigned int maxwordoffset = 0;		// size of the current wordCode array
	unsigned int wordOffset = 0;		// current location on the WordCode array

	unsigned int listlen = 0;
	unsigned int listlenmax = 0;
	unsigned int numberoflists = 0;

	unsigned int varnumber[IFTABLESIZE];	// stack of variable numbers in a statement to return the varmumber
	int nvarnumber=0;


	int functionDefSymbol = -1;	// if in a function definition (what is the symbol number) -1 = not in fundef
	int subroutineDefSymbol = -1;	// if in a subroutine definition (what is the symbol number) -1 = not in fundef

	struct label
	{
		char *name;
		int offset;
	};

	char *EMPTYSTR = "";
        char **symtable=NULL;
        int *symtableaddress=NULL;
        int *symtableaddresstype=NULL;
        int *symtableaddressargs=NULL;
        int numsyms = 0;
        int maxsymtable = 0; // size of the current symtable/symtableaddress/symtableaddresstype arrays



	// array to hold stack of if statement branch locations
	// that need to have final jump location added to them
	// the iftable is also used by for, subroutine, and function to insure
	// that no if,do,while,... is nested incorrectly
	unsigned int iftablesourceline[IFTABLESIZE];
	unsigned int iftabletype[IFTABLESIZE];
	int iftableid[IFTABLESIZE];			// used to store a sequential number for this if - unique label creation
	int iftableincludes[IFTABLESIZE];			// used to store the include depth of the code
	unsigned int iftablevariable[IFTABLESIZE];			// store the variable in a FOR to check at NEXT
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
	#define IFTABLETYPESUBROUTINE 11
	#define IFTABLETYPEINTERNAL 12


	// store the function variables here during a function definition
	unsigned int args[100];
	unsigned int argstype[100];
	int numargs = 0;

	#define ARGSTYPEVALUE 0
        #define ARGSTYPEVARARRAY 1

	// compiler workings - store in array so that interperter can display all of them
	int parsewarningtable[PARSEWARNINGTABLESIZE];
	int parsewarningtablelinenumber[PARSEWARNINGTABLESIZE];
	int parsewarningtablecolumn[PARSEWARNINGTABLESIZE];
        int parsewarningtablelexingfilenumber[PARSEWARNINGTABLESIZE];
	int numparsewarnings = 0;

        int basicParse(char *);

	void checkWordMem(unsigned int addedwords) {
		unsigned int t;
		if (wordOffset + addedwords + 1 >= maxwordoffset) {
                        maxwordoffset = maxwordoffset + addedwords + 2048;
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
		//printf("line=%i addOp op=%i\n",linenumber, op);
	}

        void addData(int data) {
                checkWordMem(1);
                wordCode[wordOffset] = data;
                wordOffset++;
        }

	void addIntOp(int op, int data) {
		addOp(op);
                addData(data);
	}

	void addIntIntOp(int op, int data, int data2) {
		addOp(op);
		addData(data);
		addData(data2);
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

	void clearIfTable() {
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
                                if (iftabletype[numifs-1]==IFTABLETYPESUBROUTINE) return COMPERR_SUBROUTINENOEND;
			}
		}
		return 0;
	}

	int newIf(int sourceline, int type, unsigned int variable) {
		iftablesourceline[numifs] = sourceline;
		iftabletype[numifs] = type;
		iftableid[numifs] = nextifid;
		iftableincludes[numifs] = numincludes;
		iftablevariable[numifs] = variable;
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
                        parsewarningtablelexingfilenumber[numparsewarnings] = filenumber;
			numparsewarnings++;
		} else {
			parsewarningtable[numparsewarnings-1] = COMPWARNING_MAXIMUMWARNINGS;
			parsewarningtablelinenumber[numparsewarnings-1] = 0;
			parsewarningtablecolumn[numparsewarnings-1] = 0;
                        parsewarningtablelexingfilenumber[numparsewarnings-1] = 0;
		}
	}

	int getSymbol(char *name) {
		// get a symbol if it exists or create a new one on the symbol table
		int i;
		for (i = 0; i < numsyms; i++) {
                        if (symtable[i] && !strcasecmp(name, symtable[i]))
				return i;
		}

                //allocate memory if there is no more room for new symbol
                if(numsyms>=maxsymtable-1){
                    maxsymtable += 1024;
                    symtable = realloc(symtable, maxsymtable * sizeof(char*));
                    symtableaddress = realloc(symtableaddress, maxsymtable * sizeof(int));
                    symtableaddresstype = realloc(symtableaddresstype, maxsymtable * sizeof(int));
                    symtableaddressargs = realloc(symtableaddressargs, maxsymtable * sizeof(int));
                }

		symtable[numsyms] = strdup(name);
		symtableaddress[numsyms] = -1;
                symtableaddresstype[numsyms] = -1;
                symtableaddressargs[numsyms] = -1;
                numsyms++;
		return numsyms - 1;
	}

	#define INTERNALSYMBOLEXIT 0 //at the end of the loop - all done
	#define INTERNALSYMBOLCONTINUE 1 //at the test of the loop
        #define INTERNALSYMBOLTOP 2 // at the top of the loop - all done

	int getInternalSymbol(int id, int type) {
		// an internal symbol used to jump an if
                int i;
		char name[32];
                sprintf(name,"___%d_%d", id, type);
                i = getSymbol(name);
                symtableaddresstype[i]=ADDRESSTYPE_SYSTEMCALL;
                return i;
	}

	void freeBasicParse() {
		// free all dynamically allocated stuff
                while(numsyms>0) free(symtable[--numsyms]);
                free(wordCode);
                wordCode=NULL;
                free(symtable);
                symtable=NULL;
                free(symtableaddress);
                symtableaddress=NULL;
                free(symtableaddresstype);
                symtableaddresstype=NULL;
                free(symtableaddressargs);
                symtableaddressargs=NULL;
                maxsymtable = 0;
                maxwordoffset = 0;

                while(include_filenames_counter>0){
                    include_filenames_counter--;
                    free(include_filenames[include_filenames_counter]);
                }
	}

        int initializeBasicParse() {
                int f;
                maxsymtable = 2048;
                symtable = malloc(maxsymtable * sizeof(char*));
                if(symtable)
                    for(f=0;f<maxsymtable;f++) symtable[f]=NULL;
                symtableaddress = malloc(maxsymtable * sizeof(int));
                symtableaddresstype = malloc(maxsymtable * sizeof(int));
                symtableaddressargs = malloc(maxsymtable * sizeof(int));

                maxwordoffset = 2048;
                wordCode = malloc(maxwordoffset * sizeof(int));

                //no memory
                if(!wordCode || !symtable || !symtableaddress || !symtableaddresstype || !symtableaddressargs){
                    freeBasicParse();
                    return -1;
                }

                unsigned int t=maxwordoffset;
                while(t>0) wordCode[--t] = 0;
                wordOffset = 0;
                linenumber = 1;
                addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
                return 0; 	// success in creating and filling
        }


	#ifdef __cplusplus
	}
	#endif




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
     B256ABS = 258,
     B256ACOS = 259,
     B256ADD1 = 260,
     B256ADDEQUAL = 261,
     B256ALERT = 262,
     B256AMP = 263,
     B256AMPEQUAL = 264,
     B256AND = 265,
     B256ARC = 266,
     B256ARRAYBASE = 267,
     B256ASC = 268,
     B256ASIN = 269,
     B256ASSIGNED = 270,
     B256ATAN = 271,
     B256BEGINCASE = 272,
     B256BINARYNOT = 273,
     B256BINARYOR = 274,
     B256BITSHIFTL = 275,
     B256BITSHIFTR = 276,
     B256BLACK = 277,
     B256BLUE = 278,
     B256BOOLEOF = 279,
     B256BOOLFALSE = 280,
     B256BOOLTRUE = 281,
     B256CALL = 282,
     B256CASE = 283,
     B256CATCH = 284,
     B256CEIL = 285,
     B256CHANGEDIR = 286,
     B256CHORD = 287,
     B256CHR = 288,
     B256CIRCLE = 289,
     B256CLEAR = 290,
     B256CLG = 291,
     B256CLICKB = 292,
     B256CLICKCLEAR = 293,
     B256CLICKX = 294,
     B256CLICKY = 295,
     B256CLOSE = 296,
     B256CLS = 297,
     B256CONFIRM = 298,
     B256CONTINUEDO = 299,
     B256CONTINUEFOR = 300,
     B256CONTINUEWHILE = 301,
     B256COS = 302,
     B256COUNT = 303,
     B256COUNTX = 304,
     B256CURRENTDIR = 305,
     B256CYAN = 306,
     B256DARKBLUE = 307,
     B256DARKCYAN = 308,
     B256DARKGREEN = 309,
     B256DARKGREY = 310,
     B256DARKORANGE = 311,
     B256DARKPURPLE = 312,
     B256DARKRED = 313,
     B256DARKYELLOW = 314,
     B256DAY = 315,
     B256DBCLOSE = 316,
     B256DBCLOSESET = 317,
     B256DBEXECUTE = 318,
     B256DBFLOAT = 319,
     B256DBINT = 320,
     B256DBNULL = 321,
     B256DBOPEN = 322,
     B256DBOPENSET = 323,
     B256DBROW = 324,
     B256DBSTRING = 325,
     B256DEBUGINFO = 326,
     B256DEGREES = 327,
     B256DIM = 328,
     B256DIR = 329,
     B256DIVEQUAL = 330,
     B256DO = 331,
     B256EDITVISIBLE = 332,
     B256ELLIPSE = 333,
     B256ELSE = 334,
     B256END = 335,
     B256ENDCASE = 336,
     B256ENDFUNCTION = 337,
     B256ENDIF = 338,
     B256ENDSUBROUTINE = 339,
     B256ENDTRY = 340,
     B256ENDWHILE = 341,
     B256ERROR_ARGUMENTCOUNT = 342,
     B256ERROR_ARRAYELEMENT = 343,
     B256ERROR_ARRAYEVEN = 344,
     B256ERROR_ARRAYEXPR = 345,
     B256ERROR_ARRAYINDEX = 346,
     B256ERROR_ARRAYINDEXMISSING = 347,
     B256ERROR_ARRAYLENGTH2D = 348,
     B256ERROR_ARRAYNITEMS = 349,
     B256ERROR_ARRAYSIZELARGE = 350,
     B256ERROR_ARRAYSIZESMALL = 351,
     B256ERROR_ASINACOSRANGE = 352,
     B256ERROR_BOOLEANCONV = 353,
     B256ERROR_DBCOLNO = 354,
     B256ERROR_DBCONNNUMBER = 355,
     B256ERROR_DBNOTOPEN = 356,
     B256ERROR_DBNOTSET = 357,
     B256ERROR_DBNOTSETROW = 358,
     B256ERROR_DBOPEN = 359,
     B256ERROR_DBQUERY = 360,
     B256ERROR_DBSETNUMBER = 361,
     B256ERROR_DIVZERO = 362,
     B256ERROR_DOWNLOAD = 363,
     B256ERROR_ENVELOPEMAX = 364,
     B256ERROR_ENVELOPEODD = 365,
     B256ERROR_EXPECTEDARRAY = 366,
     B256ERROR_EXPECTEDSOUND = 367,
     B256ERROR_FILENOTOPEN = 368,
     B256ERROR_FILENUMBER = 369,
     B256ERROR_FILEOPEN = 370,
     B256ERROR_FILEOPERATION = 371,
     B256ERROR_FILERESET = 372,
     B256ERROR_FILEWRITE = 373,
     B256ERROR_FOLDER = 374,
     B256ERROR_FREEDB = 375,
     B256ERROR_FREEDBSET = 376,
     B256ERROR_FREEFILE = 377,
     B256ERROR_FREENET = 378,
     B256ERROR_HARMONICLIST = 379,
     B256ERROR_HARMONICNUMBER = 380,
     B256ERROR_IMAGEFILE = 381,
     B256ERROR_IMAGERESOURCE = 382,
     B256ERROR_IMAGESAVETYPE = 383,
     B256ERROR_IMAGESCALE = 384,
     B256ERROR_INFINITY = 385,
     B256ERROR_INTEGERRANGE = 386,
     B256ERROR_INVALIDKEYNAME = 387,
     B256ERROR_INVALIDPROGNAME = 388,
     B256ERROR_INVALIDRESOURCE = 389,
     B256ERROR_LOGRANGE = 390,
     B256ERROR_LONGRANGE = 391,
     B256ERROR_MAXRECURSE = 392,
     B256ERROR_NETACCEPT = 393,
     B256ERROR_NETBIND = 394,
     B256ERROR_NETCONN = 395,
     B256ERROR_NETHOST = 396,
     B256ERROR_NETNONE = 397,
     B256ERROR_NETREAD = 398,
     B256ERROR_NETSOCK = 399,
     B256ERROR_NETSOCKNUMBER = 400,
     B256ERROR_NETSOCKOPT = 401,
     B256ERROR_NETWRITE = 402,
     B256ERROR_NEXTNOFOR = 403,
     B256ERROR_NONE = 404,
     B256ERROR_NOSUCHFUNCTION = 405,
     B256ERROR_NOSUCHLABEL = 406,
     B256ERROR_NOSUCHSUBROUTINE = 407,
     B256ERROR_NOTARRAY = 408,
     B256ERROR_NOTIMPLEMENTED = 409,
     B256ERROR_NUMBERCONV = 410,
     B256ERROR_NUMBEREXPR = 411,
     B256ERROR_ONEDIMENSIONAL = 412,
     B256ERROR_ONERRORSUB = 413,
     B256ERROR_PENWIDTH = 414,
     B256ERROR_PERMISSION = 415,
     B256ERROR_POLYPOINTS = 416,
     B256ERROR_PRINTERNOTOFF = 417,
     B256ERROR_PRINTERNOTON = 418,
     B256ERROR_PRINTEROPEN = 419,
     B256ERROR_RADIX = 420,
     B256ERROR_RADIXSTRING = 421,
     B256ERROR_REFNOTASSIGNED = 422,
     B256ERROR_RGB = 423,
     B256ERROR_SERIALPARAMETER = 424,
     B256ERROR_SETTINGMAXKEYS = 425,
     B256ERROR_SETTINGMAXLEN = 426,
     B256ERROR_SETTINGSGETACCESS = 427,
     B256ERROR_SETTINGSSETACCESS = 428,
     B256ERROR_SLICESIZE = 429,
     B256ERROR_SOUNDERROR = 430,
     B256ERROR_SOUNDFILE = 431,
     B256ERROR_SOUNDFILEFORMAT = 432,
     B256ERROR_SOUNDLENGTH = 433,
     B256ERROR_SOUNDNOTSEEKABLE = 434,
     B256ERROR_SOUNDRESOURCE = 435,
     B256ERROR_SPRITENA = 436,
     B256ERROR_SPRITENUMBER = 437,
     B256ERROR_SPRITESLICE = 438,
     B256ERROR_SQRRANGE = 439,
     B256ERROR_STACKUNDERFLOW = 440,
     B256ERROR_STRING2NOTE = 441,
     B256ERROR_STRINGCONV = 442,
     B256ERROR_STRINGEXPR = 443,
     B256ERROR_STRINGMAXLEN = 444,
     B256ERROR_STRSTART = 445,
     B256ERROR_TOOMANYSOUNDS = 446,
     B256ERROR_UNEXPECTEDRETURN = 447,
     B256ERROR_UNSERIALIZEFORMAT = 448,
     B256ERROR_VARCIRCULAR = 449,
     B256ERROR_VARNOTASSIGNED = 450,
     B256ERROR_VARNULL = 451,
     B256ERROR_WAVEFORMLOGICAL = 452,
     B256ERROR_WAVOBSOLETE = 453,
     B256EXISTS = 454,
     B256EXITDO = 455,
     B256EXITFOR = 456,
     B256EXITWHILE = 457,
     B256EXP = 458,
     B256EXPLODE = 459,
     B256EXPLODEX = 460,
     B256FASTGRAPHICS = 461,
     B256FILL = 462,
     B256FLOOR = 463,
     B256FONT = 464,
     B256FOR = 465,
     B256FOREACH = 466,
     B256FREEDB = 467,
     B256FREEDBSET = 468,
     B256FREEFILE = 469,
     B256FREENET = 470,
     B256FROMBINARY = 471,
     B256FROMHEX = 472,
     B256FROMOCTAL = 473,
     B256FROMRADIX = 474,
     B256FUNCTION = 475,
     B256GETBRUSHCOLOR = 476,
     B256GETCOLOR = 477,
     B256GETPENWIDTH = 478,
     B256GETSETTING = 479,
     B256GETSLICE = 480,
     B256GLOBAL = 481,
     B256GOSUB = 482,
     B256GOTO = 483,
     B256GRAPHHEIGHT = 484,
     B256GRAPHSIZE = 485,
     B256GRAPHVISIBLE = 486,
     B256GRAPHWIDTH = 487,
     B256GREEN = 488,
     B256GREY = 489,
     B256GTE = 490,
     B256HOUR = 491,
     B256IF = 492,
     B256IN = 493,
     B256IMAGEAUTOCROP = 494,
     B256IMAGECENTERED = 495,
     B256IMAGECOPY = 496,
     B256IMAGECROP = 497,
     B256IMAGEDRAW = 498,
     B256IMAGEFLIP = 499,
     B256IMAGEHEIGHT = 500,
     B256IMAGELOAD = 501,
     B256IMAGENEW = 502,
     B256IMAGEPIXEL = 503,
     B256IMAGERESIZE = 504,
     B256IMAGEROTATE = 505,
     B256IMAGESETPIXEL = 506,
     B256IMAGESMOOTH = 507,
     B256IMAGETRANSFORMED = 508,
     B256IMAGETYPE_BMP = 509,
     B256IMAGETYPE_JPG = 510,
     B256IMAGETYPE_PNG = 511,
     B256IMAGEWIDTH = 512,
     B256IMGLOAD = 513,
     B256IMGSAVE = 514,
     B256IMPLODE = 515,
     B256INPUT = 516,
     B256INPUTFLOAT = 517,
     B256INPUTINT = 518,
     B256INPUTSTRING = 519,
     B256INSTR = 520,
     B256INSTRX = 521,
     B256INTDIV = 522,
     B256ISNUMERIC = 523,
     B256KEY = 524,
     B256KEYPRESSED = 525,
     B256KILL = 526,
     B256LASTERROR = 527,
     B256LASTERROREXTRA = 528,
     B256LASTERRORLINE = 529,
     B256LASTERRORMESSAGE = 530,
     B256LEFT = 531,
     B256LENGTH = 532,
     B256LET = 533,
     B256LINE = 534,
     B256LJUST = 535,
     B256LOG = 536,
     B256LOGTEN = 537,
     B256LOWER = 538,
     B256LTE = 539,
     B256LTRIM = 540,
     B256MAP = 541,
     B256MD5 = 542,
     B256MID = 543,
     B256MIDX = 544,
     B256MINUTE = 545,
     B256MOD = 546,
     B256MONTH = 547,
     B256MOUSEB = 548,
     B256MOUSEBUTTON_CENTER = 549,
     B256MOUSEBUTTON_DOUBLECLICK = 550,
     B256MOUSEBUTTON_LEFT = 551,
     B256MOUSEBUTTON_NONE = 552,
     B256MOUSEBUTTON_RIGHT = 553,
     B256MOUSEX = 554,
     B256MOUSEY = 555,
     B256MSEC = 556,
     B256MULEQUAL = 557,
     B256NE = 558,
     B256NETADDRESS = 559,
     B256NETCLOSE = 560,
     B256NETCONNECT = 561,
     B256NETDATA = 562,
     B256NETLISTEN = 563,
     B256NETREAD = 564,
     B256NETWRITE = 565,
     B256NEXT = 566,
     B256NOT = 567,
     B256OFFERROR = 568,
     B256ONERROR = 569,
     B256OPEN = 570,
     B256OPENB = 571,
     B256OPENSERIAL = 572,
     B256OR = 573,
     B256ORANGE = 574,
     B256OSTYPE = 575,
     B256OSTYPE_ANDROID = 576,
     B256OSTYPE_LINUX = 577,
     B256OSTYPE_MACINTOSH = 578,
     B256OSTYPE_WINDOWS = 579,
     B256OUTPUTVISIBLE = 580,
     B256PAUSE = 581,
     B256PENWIDTH = 582,
     B256PI = 583,
     B256PIE = 584,
     B256PIXEL = 585,
     B256PLOT = 586,
     B256POLY = 587,
     B256PORTIN = 588,
     B256PORTOUT = 589,
     B256PRINT = 590,
     B256PRINTERCANCEL = 591,
     B256PRINTEROFF = 592,
     B256PRINTERON = 593,
     B256PRINTERPAGE = 594,
     B256PROMPT = 595,
     B256PURPLE = 596,
     B256PUTSLICE = 597,
     B256RADIANS = 598,
     B256RAND = 599,
     B256READ = 600,
     B256READBYTE = 601,
     B256READLINE = 602,
     B256RECT = 603,
     B256RED = 604,
     B256REDIM = 605,
     B256REF = 606,
     B256REFRESH = 607,
     B256REGEXMINIMAL = 608,
     B256REM = 609,
     B256REPLACE = 610,
     B256REPLACEX = 611,
     B256RESET = 612,
     B256RETURN = 613,
     B256RGB = 614,
     B256RIGHT = 615,
     B256RJUST = 616,
     B256ROUND = 617,
     B256RTRIM = 618,
     B256SAY = 619,
     B256SECOND = 620,
     B256SEED = 621,
     B256SEEK = 622,
     B256SEMICOLON = 623,
     B256SEMICOLONEQUAL = 624,
     B256SERIALIZE = 625,
     B256SETCOLOR = 626,
     B256SETGRAPH = 627,
     B256SETSETTING = 628,
     B256SIN = 629,
     B256SIZE = 630,
     B256SLICE_ALL = 631,
     B256SLICE_PAINT = 632,
     B256SLICE_SPRITE = 633,
     B256SOUND = 634,
     B256SOUNDENVELOPE = 635,
     B256SOUNDFADE = 636,
     B256SOUNDHARMONICS = 637,
     B256SOUNDID = 638,
     B256SOUNDLENGTH = 639,
     B256SOUNDLOAD = 640,
     B256SOUNDLOADRAW = 641,
     B256SOUNDLOOP = 642,
     B256SOUNDPAUSE = 643,
     B256SOUNDPLAY = 644,
     B256SOUNDPLAYER = 645,
     B256SOUNDPLAYEROFF = 646,
     B256SOUNDPOSITION = 647,
     B256SOUNDSAMPLERATE = 648,
     B256SOUNDSEEK = 649,
     B256SOUNDSTATE = 650,
     B256SOUNDSTOP = 651,
     B256SOUNDSYSTEM = 652,
     B256SOUNDVOLUME = 653,
     B256SOUNDWAIT = 654,
     B256SOUNDWAVEFORM = 655,
     B256SPRITECOLLIDE = 656,
     B256SPRITEDIM = 657,
     B256SPRITEH = 658,
     B256SPRITEHIDE = 659,
     B256SPRITELOAD = 660,
     B256SPRITEMOVE = 661,
     B256SPRITEO = 662,
     B256SPRITEPLACE = 663,
     B256SPRITEPOLY = 664,
     B256SPRITER = 665,
     B256SPRITES = 666,
     B256SPRITESHOW = 667,
     B256SPRITESLICE = 668,
     B256SPRITEV = 669,
     B256SPRITEW = 670,
     B256SPRITEX = 671,
     B256SPRITEY = 672,
     B256SQR = 673,
     B256STAMP = 674,
     B256STEP = 675,
     B256SUB1 = 676,
     B256SUBEQUAL = 677,
     B256SUBROUTINE = 678,
     B256SYSTEM = 679,
     B256TAN = 680,
     B256TEXT = 681,
     B256TEXTHEIGHT = 682,
     B256TEXTWIDTH = 683,
     B256THEN = 684,
     B256THROWERROR = 685,
     B256TO = 686,
     B256TOBINARY = 687,
     B256TOFLOAT = 688,
     B256TOHEX = 689,
     B256TOINT = 690,
     B256TOOCTAL = 691,
     B256TORADIX = 692,
     B256TOSTRING = 693,
     B256TRIM = 694,
     B256TRY = 695,
     B256TYPEOF = 696,
     B256TYPE_ARRAY = 697,
     B256TYPE_FLOAT = 698,
     B256TYPE_INT = 699,
     B256TYPE_MAP = 700,
     B256TYPE_REF = 701,
     B256TYPE_STRING = 702,
     B256TYPE_UNASSIGNED = 703,
     B256UNASSIGN = 704,
     B256UNLOAD = 705,
     B256UNSERIALIZE = 706,
     B256UNTIL = 707,
     B256UPPER = 708,
     B256VARIABLEWATCH = 709,
     B256VERSION = 710,
     B256VOLUME = 711,
     B256WARNING_ARRAYELEMENT = 712,
     B256WARNING_BOOLEANCONV = 713,
     B256WARNING_INTEGERRANGE = 714,
     B256WARNING_LONGRANGE = 715,
     B256WARNING_NUMBERCONV = 716,
     B256WARNING_REFNOTASSIGNED = 717,
     B256WARNING_SOUNDERROR = 718,
     B256WARNING_SOUNDFILEFORMAT = 719,
     B256WARNING_SOUNDLENGTH = 720,
     B256WARNING_SOUNDNOTSEEKABLE = 721,
     B256WARNING_START = 722,
     B256WARNING_STRING2NOTE = 723,
     B256WARNING_STRINGCONV = 724,
     B256WARNING_VARNOTASSIGNED = 725,
     B256WARNING_WAVOBSOLETE = 726,
     B256WAVLENGTH = 727,
     B256WAVPAUSE = 728,
     B256WAVPLAY = 729,
     B256WAVPOS = 730,
     B256WAVSEEK = 731,
     B256WAVSTATE = 732,
     B256WAVSTOP = 733,
     B256WAVWAIT = 734,
     B256WHILE = 735,
     B256WHITE = 736,
     B256WRITE = 737,
     B256WRITEBYTE = 738,
     B256WRITELINE = 739,
     B256XOR = 740,
     B256YEAR = 741,
     B256YELLOW = 742,
     B256ZFILL = 743,
     B256INTEGER = 744,
     B256FLOAT = 745,
     B256STRING = 746,
     B256HEXCONST = 747,
     B256BINCONST = 748,
     B256OCTCONST = 749,
     B256VARIABLE = 750,
     B256LABEL = 751,
     B256UNARY = 752
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
anytype
{


	int number;
	double floatnum;
	char *string;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */



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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#define YYFINAL  914
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   35126

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  517
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  924
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1951

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   752

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     513,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   516,     2,     2,
     511,   512,   503,   502,   497,   501,     2,   504,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   514,     2,
     498,   500,   499,   515,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   507,     2,   508,   506,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   509,     2,   510,     2,     2,     2,     2,
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
     495,   496,   505
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,     9,    11,    14,    18,    20,    23,
      25,    28,    30,    31,    33,    37,    39,    42,    46,    51,
      54,    59,    62,    68,    75,    79,    81,    85,    87,    90,
      94,    97,   101,   103,   106,   110,   112,   116,   120,   122,
     124,   128,   132,   134,   138,   142,   147,   154,   156,   160,
     162,   167,   170,   171,   175,   179,   183,   187,   191,   195,
     201,   205,   213,   217,   227,   231,   243,   247,   261,   265,
     283,   287,   307,   311,   313,   315,   317,   319,   321,   323,
     325,   327,   331,   335,   339,   343,   347,   352,   356,   359,
     362,   365,   368,   371,   374,   377,   380,   383,   386,   389,
     392,   395,   398,   401,   404,   407,   410,   413,   416,   419,
     422,   425,   428,   431,   434,   437,   440,   443,   446,   449,
     452,   455,   458,   461,   464,   467,   470,   473,   476,   479,
     482,   485,   488,   491,   494,   497,   500,   503,   506,   509,
     512,   515,   518,   521,   524,   527,   530,   533,   536,   539,
     542,   545,   548,   551,   554,   557,   560,   563,   566,   569,
     572,   575,   578,   581,   584,   587,   590,   593,   596,   599,
     602,   605,   608,   611,   614,   617,   620,   623,   626,   629,
     632,   635,   638,   641,   644,   647,   650,   653,   656,   659,
     662,   665,   668,   671,   674,   677,   680,   683,   686,   689,
     692,   695,   698,   701,   704,   707,   710,   713,   716,   719,
     722,   725,   728,   731,   734,   737,   740,   743,   746,   749,
     752,   755,   758,   761,   764,   767,   770,   773,   776,   779,
     782,   785,   788,   791,   794,   797,   800,   803,   806,   809,
     812,   815,   818,   821,   824,   827,   830,   833,   836,   839,
     842,   845,   848,   851,   854,   857,   859,   861,   864,   867,
     870,   874,   878,   882,   885,   889,   893,   897,   901,   905,
     909,   912,   916,   920,   924,   927,   931,   935,   939,   943,
     947,   951,   954,   957,   960,   963,   966,   969,   972,   975,
     980,   985,   990,   995,  1002,  1011,  1022,  1029,  1038,  1043,
    1048,  1053,  1058,  1063,  1068,  1073,  1078,  1083,  1088,  1093,
    1098,  1103,  1108,  1113,  1116,  1119,  1122,  1127,  1132,  1135,
    1138,  1141,  1144,  1147,  1150,  1153,  1156,  1159,  1164,  1167,
    1172,  1175,  1178,  1181,  1184,  1187,  1190,  1193,  1200,  1209,
    1220,  1223,  1226,  1229,  1238,  1245,  1250,  1255,  1260,  1265,
    1270,  1275,  1280,  1285,  1288,  1293,  1300,  1305,  1312,  1321,
    1326,  1333,  1342,  1347,  1354,  1363,  1366,  1369,  1372,  1377,
    1382,  1389,  1398,  1405,  1408,  1411,  1416,  1423,  1426,  1433,
    1436,  1441,  1444,  1447,  1452,  1455,  1458,  1461,  1466,  1473,
    1478,  1483,  1488,  1495,  1497,  1499,  1501,  1504,  1507,  1510,
    1515,  1520,  1525,  1528,  1533,  1536,  1541,  1544,  1549,  1552,
    1555,  1560,  1565,  1570,  1575,  1580,  1585,  1590,  1593,  1596,
    1599,  1601,  1606,  1611,  1616,  1621,  1630,  1637,  1646,  1653,
    1660,  1663,  1668,  1671,  1676,  1679,  1684,  1691,  1700,  1703,
    1706,  1709,  1714,  1717,  1722,  1729,  1734,  1737,  1746,  1757,
    1766,  1771,  1776,  1781,  1786,  1791,  1798,  1803,  1808,  1813,
    1820,  1825,  1830,  1835,  1840,  1845,  1848,  1851,  1854,  1859,
    1864,  1869,  1878,  1885,  1890,  1903,  1914,  1919,  1922,  1927,
    1932,  1937,  1942,  1947,  1949,  1951,  1953,  1958,  1961,  1964,
    1967,  1970,  1973,  1975,  1977,  1979,  1981,  1983,  1985,  1987,
    1989,  1991,  1993,  1995,  1997,  1999,  2001,  2003,  2005,  2007,
    2009,  2011,  2013,  2015,  2017,  2019,  2021,  2023,  2025,  2027,
    2029,  2031,  2033,  2035,  2037,  2039,  2041,  2043,  2045,  2047,
    2049,  2051,  2053,  2055,  2057,  2059,  2061,  2063,  2065,  2067,
    2069,  2071,  2073,  2075,  2077,  2079,  2081,  2083,  2085,  2087,
    2089,  2091,  2093,  2095,  2097,  2099,  2101,  2103,  2105,  2107,
    2109,  2111,  2113,  2115,  2117,  2119,  2121,  2123,  2125,  2127,
    2129,  2131,  2133,  2135,  2137,  2139,  2141,  2143,  2145,  2147,
    2149,  2151,  2153,  2155,  2157,  2159,  2161,  2163,  2165,  2167,
    2169,  2171,  2173,  2175,  2177,  2179,  2181,  2183,  2185,  2187,
    2189,  2191,  2193,  2195,  2197,  2199,  2201,  2203,  2205,  2207,
    2209,  2211,  2213,  2215,  2217,  2219,  2221,  2223,  2225,  2227,
    2229,  2231,  2233,  2235,  2237,  2239,  2241,  2243,  2245,  2247,
    2249,  2251,  2253,  2255,  2257,  2258,  2262,  2264,  2266,  2268,
    2270,  2272,  2274,  2276,  2280,  2283,  2286,  2290,  2292,  2293,
    2297,  2298,  2302,  2305,  2308,  2311,  2313,  2315,  2317,  2320,
    2323,  2328,  2332,  2338,  2342,  2348,  2353,  2358,  2361,  2366,
    2370,  2376,  2380,  2386,  2389,  2392,  2395,  2398,  2401,  2404,
    2407,  2410,  2413,  2416,  2420,  2423,  2426,  2430,  2434,  2438,
    2442,  2446,  2450,  2454,  2458,  2461,  2464,  2468,  2472,  2476,
    2480,  2484,  2488,  2495,  2504,  2509,  2517,  2520,  2522,  2525,
    2528,  2533,  2539,  2542,  2545,  2550,  2556,  2559,  2562,  2565,
    2568,  2571,  2574,  2577,  2580,  2583,  2586,  2589,  2592,  2595,
    2598,  2601,  2604,  2607,  2610,  2613,  2616,  2619,  2622,  2625,
    2628,  2631,  2634,  2637,  2640,  2643,  2646,  2649,  2652,  2655,
    2658,  2661,  2664,  2667,  2670,  2673,  2676,  2679,  2682,  2685,
    2688,  2691,  2694,  2697,  2700,  2703,  2706,  2709,  2712,  2715,
    2718,  2721,  2724,  2727,  2730,  2733,  2736,  2739,  2742,  2745,
    2748,  2751,  2754,  2757,  2760,  2763,  2766,  2769,  2772,  2775,
    2778,  2781,  2784,  2787,  2790,  2793,  2796,  2799,  2802,  2805,
    2808,  2811,  2814,  2817,  2820,  2823,  2826,  2829,  2832,  2835,
    2838,  2841,  2844,  2847,  2850,  2853,  2856,  2859,  2862,  2866,
    2869,  2874,  2877,  2881,  2884,  2889,  2892,  2895,  2898,  2901,
    2904,  2907,  2910,  2913,  2916,  2919,  2922,  2925,  2928,  2931,
    2934,  2937,  2940,  2943,  2946,  2949,  2952,  2955,  2958,  2961,
    2964,  2967,  2970,  2973,  2976,  2979,  2982,  2985,  2988,  2991,
    2994,  2997,  3000,  3003,  3006,  3009,  3012,  3015,  3018,  3021,
    3024,  3027,  3030,  3033,  3036,  3039,  3042,  3045,  3048,  3051,
    3054,  3057,  3060,  3062,  3064,  3066,  3068,  3070,  3072,  3075,
    3078,  3081,  3084,  3088,  3092,  3094,  3096,  3099,  3102,  3105,
    3108,  3111,  3114,  3117,  3120,  3123,  3126,  3129,  3132,  3135,
    3138,  3141,  3144,  3147,  3150,  3153,  3156,  3159,  3162,  3165,
    3168,  3171,  3174,  3177,  3180
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     518,     0,    -1,   520,   519,   518,    -1,   520,    -1,   513,
      -1,   521,   522,    -1,   521,   522,   354,    -1,   522,    -1,
     522,   354,    -1,   521,    -1,   521,   354,    -1,   354,    -1,
      -1,   496,    -1,   561,   514,   522,    -1,   561,    -1,   507,
     508,    -1,   507,   515,   508,    -1,   507,   515,   497,   508,
      -1,   524,   523,    -1,   507,   497,   515,   508,    -1,   523,
     524,    -1,   507,   553,   497,   553,   508,    -1,   507,   553,
     508,   507,   553,   508,    -1,   507,   553,   508,    -1,   495,
      -1,   511,   528,   512,    -1,   528,    -1,   528,   523,    -1,
     511,   529,   512,    -1,   528,   527,    -1,   511,   530,   512,
      -1,   529,    -1,   511,   512,    -1,   511,   533,   512,    -1,
     531,    -1,   531,   497,   533,    -1,   509,   535,   510,    -1,
     537,    -1,   536,    -1,   536,   497,   535,    -1,   509,   537,
     510,    -1,   553,    -1,   553,   497,   537,    -1,   509,   539,
     510,    -1,   553,   501,   499,   553,    -1,   553,   501,   499,
     553,   497,   539,    -1,   541,    -1,   541,   497,   540,    -1,
     553,    -1,   351,   511,   528,   512,    -1,   511,   512,    -1,
      -1,   553,   497,   553,    -1,   511,   543,   512,    -1,   553,
     497,   530,    -1,   511,   544,   512,    -1,   553,   497,   528,
      -1,   511,   545,   512,    -1,   553,   497,   553,   497,   553,
      -1,   511,   546,   512,    -1,   553,   497,   553,   497,   553,
     497,   553,    -1,   511,   547,   512,    -1,   553,   497,   553,
     497,   553,   497,   553,   497,   553,    -1,   511,   548,   512,
      -1,   553,   497,   553,   497,   553,   497,   553,   497,   553,
     497,   553,    -1,   511,   549,   512,    -1,   553,   497,   553,
     497,   553,   497,   553,   497,   553,   497,   553,   497,   553,
      -1,   511,   550,   512,    -1,   553,   497,   553,   497,   553,
     497,   553,   497,   553,   497,   553,   497,   553,   497,   553,
     497,   553,    -1,   511,   551,   512,    -1,   553,   497,   553,
     497,   553,   497,   553,   497,   553,   497,   553,   497,   553,
     497,   553,   497,   553,   497,   553,    -1,   511,   552,   512,
      -1,   554,    -1,   555,    -1,   556,    -1,   557,    -1,   558,
      -1,   559,    -1,   560,    -1,   530,    -1,   511,   553,   512,
      -1,   553,   502,   553,    -1,   553,   368,   553,    -1,   553,
       8,   553,    -1,   553,   503,   553,    -1,   528,   511,   540,
     512,    -1,   528,   511,   512,    -1,    22,   542,    -1,    23,
     542,    -1,    25,   542,    -1,    26,   542,    -1,    35,   542,
      -1,    51,   542,    -1,    52,   542,    -1,    53,   542,    -1,
      54,   542,    -1,    55,   542,    -1,    56,   542,    -1,    57,
     542,    -1,    58,   542,    -1,    59,   542,    -1,   233,   542,
      -1,   234,   542,    -1,   294,   542,    -1,   295,   542,    -1,
     296,   542,    -1,   297,   542,    -1,   298,   542,    -1,   319,
     542,    -1,   321,   542,    -1,   322,   542,    -1,   323,   542,
      -1,   324,   542,    -1,   341,   542,    -1,   349,   542,    -1,
     376,   542,    -1,   377,   542,    -1,   378,   542,    -1,   442,
     542,    -1,   443,   542,    -1,   444,   542,    -1,   445,   542,
      -1,   446,   542,    -1,   447,   542,    -1,   448,   542,    -1,
     481,   542,    -1,   487,   542,    -1,    87,   542,    -1,    88,
     542,    -1,    89,   542,    -1,    90,   542,    -1,    91,   542,
      -1,    92,   542,    -1,    93,   542,    -1,    94,   542,    -1,
      95,   542,    -1,    96,   542,    -1,    97,   542,    -1,    98,
     542,    -1,    99,   542,    -1,   100,   542,    -1,   101,   542,
      -1,   102,   542,    -1,   103,   542,    -1,   104,   542,    -1,
     105,   542,    -1,   106,   542,    -1,   107,   542,    -1,   108,
     542,    -1,   109,   542,    -1,   110,   542,    -1,   111,   542,
      -1,   112,   542,    -1,   113,   542,    -1,   114,   542,    -1,
     115,   542,    -1,   116,   542,    -1,   117,   542,    -1,   118,
     542,    -1,   119,   542,    -1,   120,   542,    -1,   121,   542,
      -1,   122,   542,    -1,   123,   542,    -1,   124,   542,    -1,
     125,   542,    -1,   126,   542,    -1,   127,   542,    -1,   128,
     542,    -1,   129,   542,    -1,   130,   542,    -1,   131,   542,
      -1,   132,   542,    -1,   133,   542,    -1,   134,   542,    -1,
     135,   542,    -1,   136,   542,    -1,   137,   542,    -1,   138,
     542,    -1,   139,   542,    -1,   140,   542,    -1,   141,   542,
      -1,   142,   542,    -1,   143,   542,    -1,   144,   542,    -1,
     145,   542,    -1,   146,   542,    -1,   147,   542,    -1,   148,
     542,    -1,   149,   542,    -1,   150,   542,    -1,   151,   542,
      -1,   152,   542,    -1,   153,   542,    -1,   154,   542,    -1,
     155,   542,    -1,   156,   542,    -1,   157,   542,    -1,   158,
     542,    -1,   159,   542,    -1,   160,   542,    -1,   161,   542,
      -1,   162,   542,    -1,   163,   542,    -1,   164,   542,    -1,
     165,   542,    -1,   166,   542,    -1,   167,   542,    -1,   168,
     542,    -1,   169,   542,    -1,   170,   542,    -1,   171,   542,
      -1,   172,   542,    -1,   173,   542,    -1,   174,   542,    -1,
     175,   542,    -1,   176,   542,    -1,   177,   542,    -1,   178,
     542,    -1,   179,   542,    -1,   180,   542,    -1,   181,   542,
      -1,   182,   542,    -1,   183,   542,    -1,   184,   542,    -1,
     185,   542,    -1,   186,   542,    -1,   187,   542,    -1,   188,
     542,    -1,   189,   542,    -1,   190,   542,    -1,   191,   542,
      -1,   192,   542,    -1,   193,   542,    -1,   194,   542,    -1,
     195,   542,    -1,   196,   542,    -1,   197,   542,    -1,   198,
     542,    -1,   457,   542,    -1,   458,   542,    -1,   459,   542,
      -1,   460,   542,    -1,   461,   542,    -1,   462,   542,    -1,
     463,   542,    -1,   464,   542,    -1,   465,   542,    -1,   466,
     542,    -1,   467,   542,    -1,   468,   542,    -1,   469,   542,
      -1,   470,   542,    -1,   471,   542,    -1,   489,    -1,   490,
      -1,   502,   489,    -1,   502,   490,    -1,   501,   553,    -1,
     553,   501,   553,    -1,   553,   291,   553,    -1,   553,   516,
     553,    -1,   553,   516,    -1,   553,   267,   553,    -1,   553,
     504,   553,    -1,   553,   506,   553,    -1,   553,    19,   553,
      -1,   553,    20,   553,    -1,   553,    21,   553,    -1,    18,
     553,    -1,   553,    10,   553,    -1,   553,   318,   553,    -1,
     553,   485,   553,    -1,   312,   553,    -1,   553,   500,   553,
      -1,   553,   303,   553,    -1,   553,   498,   553,    -1,   553,
     499,   553,    -1,   553,   235,   553,    -1,   553,   284,   553,
      -1,   530,     5,    -1,   530,   421,    -1,     5,   530,    -1,
     421,   530,    -1,   528,     5,    -1,   528,   421,    -1,     5,
     528,    -1,   421,   528,    -1,   435,   511,   553,   512,    -1,
     433,   511,   553,   512,    -1,   277,   511,   553,   512,    -1,
      13,   511,   553,   512,    -1,   265,   511,   553,   497,   553,
     512,    -1,   265,   511,   553,   497,   553,   497,   553,   512,
      -1,   265,   511,   553,   497,   553,   497,   553,   497,   553,
     512,    -1,   266,   511,   553,   497,   553,   512,    -1,   266,
     511,   553,   497,   553,   497,   553,   512,    -1,    30,   511,
     553,   512,    -1,   208,   511,   553,   512,    -1,   374,   511,
     553,   512,    -1,    47,   511,   553,   512,    -1,   425,   511,
     553,   512,    -1,    14,   511,   553,   512,    -1,     4,   511,
     553,   512,    -1,    16,   511,   553,   512,    -1,    72,   511,
     553,   512,    -1,   343,   511,   553,   512,    -1,   281,   511,
     553,   512,    -1,   282,   511,   553,   512,    -1,   418,   511,
     553,   512,    -1,   203,   511,   553,   512,    -1,     3,   511,
     553,   512,    -1,   344,   542,    -1,   328,   542,    -1,    24,
     542,    -1,    24,   511,   553,   512,    -1,   199,   511,   553,
     512,    -1,   486,   542,    -1,   292,   542,    -1,    60,   542,
      -1,   236,   542,    -1,   290,   542,    -1,   365,   542,    -1,
     232,   542,    -1,   229,   542,    -1,   375,   542,    -1,   375,
     511,   553,   512,    -1,   270,   542,    -1,   270,   511,   553,
     512,    -1,   269,   542,    -1,   299,   542,    -1,   300,   542,
      -1,   293,   542,    -1,    39,   542,    -1,    40,   542,    -1,
      37,   542,    -1,   330,   511,   553,   497,   553,   512,    -1,
     359,   511,   553,   497,   553,   497,   553,   512,    -1,   359,
     511,   553,   497,   553,   497,   553,   497,   553,   512,    -1,
     222,   542,    -1,   221,   542,    -1,   223,   542,    -1,   401,
     511,   553,   497,   553,   497,   553,   512,    -1,   401,   511,
     553,   497,   553,   512,    -1,   416,   511,   553,   512,    -1,
     417,   511,   553,   512,    -1,   403,   511,   553,   512,    -1,
     415,   511,   553,   512,    -1,   414,   511,   553,   512,    -1,
     410,   511,   553,   512,    -1,   411,   511,   553,   512,    -1,
     407,   511,   553,   512,    -1,    69,   542,    -1,    69,   511,
     553,   512,    -1,    69,   511,   553,   497,   553,   512,    -1,
      65,   511,   553,   512,    -1,    65,   511,   553,   497,   553,
     512,    -1,    65,   511,   553,   497,   553,   497,   553,   512,
      -1,    64,   511,   553,   512,    -1,    64,   511,   553,   497,
     553,   512,    -1,    64,   511,   553,   497,   553,   497,   553,
     512,    -1,    66,   511,   553,   512,    -1,    66,   511,   553,
     497,   553,   512,    -1,    66,   511,   553,   497,   553,   497,
     553,   512,    -1,   272,   542,    -1,   274,   542,    -1,   307,
     542,    -1,   307,   511,   553,   512,    -1,   333,   511,   553,
     512,    -1,    48,   511,   553,   497,   553,   512,    -1,    48,
     511,   553,   497,   553,   497,   553,   512,    -1,    49,   511,
     553,   497,   553,   512,    -1,   320,   542,    -1,   301,   542,
      -1,   428,   511,   553,   512,    -1,   428,   511,   553,   497,
     553,   512,    -1,   427,   542,    -1,   427,   511,   553,   497,
     553,   512,    -1,   346,   542,    -1,   346,   511,   553,   512,
      -1,   212,   542,    -1,   213,   542,    -1,   213,   511,   553,
     512,    -1,   214,   542,    -1,   215,   542,    -1,   455,   542,
      -1,    43,   511,   553,   512,    -1,    43,   511,   553,   497,
     553,   512,    -1,   216,   511,   553,   512,    -1,   217,   511,
     553,   512,    -1,   218,   511,   553,   512,    -1,   219,   511,
     553,   497,   553,   512,    -1,   493,    -1,   492,    -1,   494,
      -1,   472,   542,    -1,   475,   542,    -1,   477,   542,    -1,
     441,   511,   553,   512,    -1,   390,   511,   553,   512,    -1,
     390,   511,   543,   512,    -1,   383,   542,    -1,   392,   511,
     553,   512,    -1,   392,   542,    -1,   395,   511,   553,   512,
      -1,   395,   542,    -1,   384,   511,   553,   512,    -1,   384,
     542,    -1,   393,   542,    -1,   257,   511,   553,   512,    -1,
     245,   511,   553,   512,    -1,   248,   511,   546,   512,    -1,
     362,   511,   553,   512,    -1,   362,   511,   543,   512,    -1,
      15,   511,   529,   512,    -1,    15,   511,   530,   512,    -1,
     528,   524,    -1,   528,   525,    -1,   528,   526,    -1,   491,
      -1,    33,   511,   553,   512,    -1,   438,   511,   553,   512,
      -1,   453,   511,   553,   512,    -1,   283,   511,   553,   512,
      -1,   288,   511,   553,   497,   553,   497,   553,   512,    -1,
     289,   511,   553,   497,   553,   512,    -1,   289,   511,   553,
     497,   553,   497,   553,   512,    -1,   276,   511,   553,   497,
     553,   512,    -1,   360,   511,   553,   497,   553,   512,    -1,
     345,   542,    -1,   345,   511,   553,   512,    -1,   347,   542,
      -1,   347,   511,   553,   512,    -1,    50,   542,    -1,    70,
     511,   553,   512,    -1,    70,   511,   553,   497,   553,   512,
      -1,    70,   511,   553,   497,   553,   497,   553,   512,    -1,
     275,   542,    -1,   273,   542,    -1,   309,   542,    -1,   309,
     511,   553,   512,    -1,   304,   542,    -1,   287,   511,   553,
     512,    -1,   224,   511,   553,   497,   553,   512,    -1,    74,
     511,   553,   512,    -1,    74,   542,    -1,   355,   511,   553,
     497,   553,   497,   553,   512,    -1,   355,   511,   553,   497,
     553,   497,   553,   497,   553,   512,    -1,   356,   511,   553,
     497,   553,   497,   553,   512,    -1,   370,   511,   553,   512,
      -1,   260,   511,   553,   512,    -1,   260,   511,   543,   512,
      -1,   260,   511,   546,   512,    -1,   340,   511,   553,   512,
      -1,   340,   511,   553,   497,   553,   512,    -1,   432,   511,
     553,   512,    -1,   434,   511,   553,   512,    -1,   436,   511,
     553,   512,    -1,   437,   511,   553,   497,   553,   512,    -1,
      71,   511,   553,   512,    -1,   268,   511,   553,   512,    -1,
     285,   511,   553,   512,    -1,   363,   511,   553,   512,    -1,
     439,   511,   553,   512,    -1,   254,   542,    -1,   255,   542,
      -1,   256,   542,    -1,   385,   511,   553,   512,    -1,   385,
     511,   543,   512,    -1,   386,   511,   553,   512,    -1,   247,
     511,   553,   497,   553,   497,   553,   512,    -1,   247,   511,
     553,   497,   553,   512,    -1,   246,   511,   553,   512,    -1,
     241,   511,   553,   497,   553,   497,   553,   497,   553,   497,
     553,   512,    -1,   241,   511,   553,   497,   553,   497,   553,
     497,   553,   512,    -1,   241,   511,   553,   512,    -1,   241,
     542,    -1,   280,   511,   543,   512,    -1,   280,   511,   546,
     512,    -1,   361,   511,   543,   512,    -1,   361,   511,   546,
     512,    -1,   488,   511,   543,   512,    -1,   529,    -1,   534,
      -1,   538,    -1,   451,   511,   553,   512,    -1,   204,   543,
      -1,   204,   546,    -1,   205,   543,    -1,   225,   547,    -1,
     225,   548,    -1,   679,    -1,   623,    -1,   709,    -1,   562,
      -1,   600,    -1,   563,    -1,   565,    -1,   660,    -1,   624,
      -1,   621,    -1,   587,    -1,   659,    -1,   638,    -1,   604,
      -1,   680,    -1,   681,    -1,   682,    -1,   665,    -1,   662,
      -1,   663,    -1,   664,    -1,   661,    -1,   583,    -1,   566,
      -1,   708,    -1,   674,    -1,   622,    -1,   567,    -1,   568,
      -1,   692,    -1,   569,    -1,   591,    -1,   693,    -1,   570,
      -1,   571,    -1,   683,    -1,   684,    -1,   685,    -1,   588,
      -1,   628,    -1,   595,    -1,   596,    -1,   690,    -1,   677,
      -1,   599,    -1,   598,    -1,   589,    -1,   675,    -1,   572,
      -1,   573,    -1,   574,    -1,   698,    -1,   702,    -1,   697,
      -1,   701,    -1,   705,    -1,   700,    -1,   699,    -1,   704,
      -1,   706,    -1,   703,    -1,   650,    -1,   673,    -1,   642,
      -1,   670,    -1,   581,    -1,   620,    -1,   582,    -1,   669,
      -1,   667,    -1,   666,    -1,   668,    -1,   597,    -1,   601,
      -1,   602,    -1,   634,    -1,   676,    -1,   585,    -1,   678,
      -1,   625,    -1,   619,    -1,   632,    -1,   672,    -1,   686,
      -1,   687,    -1,   688,    -1,   689,    -1,   643,    -1,   649,
      -1,   626,    -1,   584,    -1,   590,    -1,   694,    -1,   639,
      -1,   603,    -1,   629,    -1,   640,    -1,   641,    -1,   671,
      -1,   605,    -1,   607,    -1,   608,    -1,   606,    -1,   609,
      -1,   610,    -1,   611,    -1,   612,    -1,   613,    -1,   614,
      -1,   615,    -1,   616,    -1,   617,    -1,   618,    -1,   651,
      -1,   657,    -1,   652,    -1,   656,    -1,   655,    -1,   654,
      -1,   658,    -1,   653,    -1,   633,    -1,   691,    -1,   630,
      -1,   627,    -1,   586,    -1,   576,    -1,   695,    -1,   707,
      -1,   577,    -1,   696,    -1,   631,    -1,   644,    -1,   645,
      -1,   646,    -1,   647,    -1,   648,    -1,   579,    -1,   637,
      -1,   636,    -1,   635,    -1,    17,    -1,    -1,    28,   564,
     553,    -1,    29,    -1,    76,    -1,    79,    -1,    81,    -1,
      83,    -1,    85,    -1,    86,    -1,   237,   553,   429,    -1,
     572,   522,    -1,   575,   522,    -1,   572,   522,    79,    -1,
     440,    -1,    -1,   452,   578,   553,    -1,    -1,   480,   580,
     553,    -1,   278,   594,    -1,   278,   593,    -1,   278,   592,
      -1,   594,    -1,   593,    -1,   592,    -1,   286,   533,    -1,
      73,   530,    -1,    73,   530,   207,   553,    -1,    73,   529,
     553,    -1,    73,   529,   553,   207,   553,    -1,    73,   529,
     543,    -1,    73,   529,   543,   207,   553,    -1,    73,   529,
     500,   553,    -1,    73,   529,   207,   553,    -1,   350,   530,
      -1,   350,   530,   207,   553,    -1,   350,   529,   553,    -1,
     350,   529,   553,   207,   553,    -1,   350,   529,   543,    -1,
     350,   529,   543,   207,   553,    -1,   326,   553,    -1,   430,
     553,    -1,    42,   542,    -1,    36,   542,    -1,    36,   553,
      -1,   206,   542,    -1,   230,   543,    -1,   230,   546,    -1,
     352,   542,    -1,    80,   542,    -1,   530,   500,   553,    -1,
     530,     5,    -1,   530,   421,    -1,   530,     6,   553,    -1,
     530,   422,   553,    -1,   530,   302,   553,    -1,   530,    75,
     553,    -1,   530,     9,   553,    -1,   530,   369,   553,    -1,
     529,   207,   553,    -1,   529,   500,   553,    -1,   528,     5,
      -1,   528,   421,    -1,   528,     6,   553,    -1,   528,   422,
     553,    -1,   528,   302,   553,    -1,   528,    75,   553,    -1,
     528,     9,   553,    -1,   528,   369,   553,    -1,   210,   528,
     500,   553,   431,   553,    -1,   210,   528,   500,   553,   431,
     553,   420,   553,    -1,   211,   528,   238,   560,    -1,   211,
     528,   501,   499,   528,   238,   560,    -1,   311,   528,    -1,
     311,    -1,   228,   528,    -1,   227,   528,    -1,    27,   528,
     511,   512,    -1,    27,   528,   511,   540,   512,    -1,   313,
     542,    -1,   314,   528,    -1,   314,   528,   511,   512,    -1,
     314,   528,   511,   540,   512,    -1,   358,   542,    -1,   358,
     553,    -1,   371,   553,    -1,   371,   543,    -1,   371,   546,
      -1,   379,   543,    -1,   379,   553,    -1,   389,   542,    -1,
     389,   543,    -1,   389,   553,    -1,   388,   553,    -1,   388,
     542,    -1,   391,   553,    -1,   391,   542,    -1,   396,   553,
      -1,   396,   542,    -1,   399,   553,    -1,   399,   542,    -1,
     400,   553,    -1,   400,   543,    -1,   397,   553,    -1,   380,
     542,    -1,   380,   547,    -1,   380,   553,    -1,   382,   542,
      -1,   382,   543,    -1,   382,   553,    -1,   381,   547,    -1,
     381,   546,    -1,   394,   543,    -1,   394,   553,    -1,   398,
     543,    -1,   398,   553,    -1,   387,   543,    -1,   387,   553,
      -1,   331,   543,    -1,   279,   547,    -1,    34,   546,    -1,
      78,   547,    -1,    11,   548,    -1,    11,   549,    -1,    32,
     548,    -1,    32,   549,    -1,   329,   548,    -1,   329,   549,
      -1,   348,   547,    -1,   348,   548,    -1,   348,   549,    -1,
     426,   546,    -1,   426,   548,    -1,   426,   549,    -1,   209,
     547,    -1,   209,   546,    -1,   209,   543,    -1,   209,   553,
      -1,   364,   553,    -1,   424,   553,    -1,   456,   553,    -1,
     332,   553,    -1,   419,   548,    -1,   419,   547,    -1,   419,
     546,    -1,   315,   553,    -1,   315,   543,    -1,   316,   553,
      -1,   316,   543,    -1,   317,   543,    -1,   317,   546,    -1,
     317,   547,    -1,   317,   548,    -1,   317,   549,    -1,   317,
     550,    -1,   482,   553,    -1,   482,   543,    -1,   484,   553,
      -1,   484,   543,    -1,   483,   553,    -1,   483,   543,    -1,
      41,   542,    -1,    41,   553,    -1,   357,   542,    -1,   357,
     553,    -1,   366,   553,    -1,   367,   553,    -1,   367,   543,
      -1,   261,   545,    -1,   261,   528,    -1,   261,   544,    -1,
     261,   530,    -1,   264,   545,    -1,   264,   528,    -1,   264,
     544,    -1,   264,   530,    -1,   263,   545,    -1,   263,   528,
      -1,   263,   544,    -1,   263,   530,    -1,   262,   545,    -1,
     262,   528,    -1,   262,   544,    -1,   262,   530,    -1,   335,
     542,    -1,   335,   553,   368,    -1,   335,   537,    -1,   335,
     511,   537,   512,    -1,   515,   542,    -1,   515,   553,   368,
      -1,   515,   537,    -1,   515,   511,   537,   512,    -1,   473,
     542,    -1,   474,   542,    -1,   474,   553,    -1,   476,   553,
      -1,   478,   542,    -1,   479,   542,    -1,   342,   546,    -1,
     258,   546,    -1,   258,   547,    -1,   258,   548,    -1,   402,
     553,    -1,   405,   543,    -1,   413,   548,    -1,   409,   543,
      -1,   408,   546,    -1,   408,   547,    -1,   408,   548,    -1,
     408,   549,    -1,   406,   546,    -1,   406,   547,    -1,   406,
     548,    -1,   406,   549,    -1,   404,   553,    -1,   412,   553,
      -1,    38,   542,    -1,    31,   553,    -1,    67,   553,    -1,
      67,   543,    -1,    61,   542,    -1,    61,   553,    -1,    63,
     553,    -1,    63,   543,    -1,    68,   553,    -1,    68,   543,
      -1,    68,   546,    -1,    62,   542,    -1,    62,   553,    -1,
      62,   543,    -1,   308,   553,    -1,   308,   543,    -1,   306,
     543,    -1,   306,   546,    -1,   310,   553,    -1,   310,   543,
      -1,   305,   542,    -1,   305,   553,    -1,   271,   553,    -1,
     373,   546,    -1,   334,   543,    -1,   259,   553,    -1,   259,
     543,    -1,    77,   553,    -1,   231,   553,    -1,   325,   553,
      -1,   226,   533,    -1,   327,   553,    -1,     7,   553,    -1,
      44,    -1,    45,    -1,    46,    -1,   200,    -1,   201,    -1,
     202,    -1,   336,   542,    -1,   337,   542,    -1,   338,   542,
      -1,   339,   542,    -1,   220,   528,   532,    -1,   423,   528,
     532,    -1,    82,    -1,    84,    -1,   353,   553,    -1,   449,
     529,    -1,   449,   530,    -1,   454,   529,    -1,   242,   548,
      -1,   239,   553,    -1,   239,   543,    -1,   249,   546,    -1,
     249,   543,    -1,   251,   547,    -1,   251,   546,    -1,   243,
     549,    -1,   243,   548,    -1,   243,   547,    -1,   243,   546,
      -1,   240,   549,    -1,   240,   548,    -1,   240,   547,    -1,
     240,   546,    -1,   253,   552,    -1,   253,   551,    -1,   250,
     543,    -1,   244,   546,    -1,   244,   543,    -1,   252,   553,
      -1,   450,   553,    -1,   372,   553,    -1,   372,   542,    -1,
      12,   553,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   880,   880,   881,   885,   893,   894,   895,   896,   897,
     898,   899,   900,   904,   922,   923,   936,   940,   944,   945,
     949,   950,   955,   956,   957,   964,   967,   971,   972,   973,
     977,   978,   988,   995,   996,  1001,  1002,  1015,  1027,  1033,
    1034,  1040,  1049,  1050,  1056,  1065,  1066,  1072,  1073,  1079,
    1080,  1094,  1095,  1102,  1103,  1107,  1108,  1112,  1113,  1118,
    1119,  1124,  1125,  1130,  1131,  1136,  1137,  1142,  1143,  1148,
    1149,  1154,  1155,  1160,  1161,  1162,  1163,  1164,  1165,  1166,
    1167,  1177,  1178,  1181,  1184,  1187,  1195,  1201,  1216,  1217,
    1218,  1219,  1220,  1221,  1222,  1223,  1224,  1225,  1226,  1227,
    1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,  1237,
    1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,
    1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,  1263,  1266,
    1269,  1272,  1275,  1278,  1281,  1284,  1287,  1290,  1293,  1296,
    1299,  1302,  1305,  1308,  1311,  1314,  1317,  1320,  1323,  1326,
    1329,  1332,  1335,  1338,  1341,  1344,  1347,  1350,  1353,  1356,
    1359,  1362,  1365,  1368,  1371,  1374,  1377,  1380,  1383,  1386,
    1389,  1392,  1395,  1398,  1401,  1404,  1407,  1410,  1413,  1416,
    1419,  1422,  1425,  1428,  1431,  1434,  1437,  1440,  1443,  1446,
    1449,  1452,  1455,  1458,  1461,  1464,  1467,  1470,  1473,  1476,
    1479,  1482,  1485,  1488,  1491,  1494,  1497,  1500,  1503,  1506,
    1509,  1512,  1515,  1518,  1521,  1524,  1527,  1530,  1533,  1536,
    1539,  1542,  1545,  1548,  1551,  1554,  1557,  1560,  1563,  1566,
    1569,  1572,  1575,  1578,  1581,  1584,  1587,  1590,  1593,  1596,
    1599,  1602,  1605,  1608,  1611,  1614,  1617,  1620,  1623,  1626,
    1629,  1632,  1635,  1638,  1641,  1651,  1652,  1661,  1666,  1675,
    1679,  1682,  1685,  1688,  1693,  1696,  1699,  1700,  1701,  1702,
    1703,  1704,  1707,  1710,  1711,  1712,  1713,  1714,  1715,  1716,
    1717,  1719,  1731,  1743,  1755,  1767,  1774,  1781,  1788,  1795,
    1796,  1797,  1798,  1799,  1804,  1808,  1809,  1813,  1814,  1815,
    1816,  1817,  1818,  1819,  1820,  1821,  1822,  1823,  1824,  1825,
    1826,  1827,  1828,  1829,  1830,  1831,  1835,  1836,  1837,  1838,
    1839,  1840,  1841,  1842,  1843,  1844,  1845,  1849,  1850,  1854,
    1855,  1856,  1857,  1858,  1859,  1860,  1861,  1862,  1863,  1867,
    1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,  1878,  1879,
    1880,  1881,  1882,  1883,  1888,  1892,  1895,  1901,  1905,  1907,
    1913,  1917,  1919,  1925,  1929,  1931,  1932,  1933,  1934,  1935,
    1936,  1940,  1941,  1942,  1943,  1944,  1945,  1946,  1947,  1948,
    1949,  1950,  1951,  1955,  1956,  1957,  1958,  1959,  1963,  1966,
    1970,  1974,  1978,  1981,  1991,  2001,  2011,  2012,  2013,  2015,
    2019,  2022,  2028,  2031,  2034,  2038,  2041,  2045,  2048,  2052,
    2055,  2058,  2061,  2064,  2068,  2071,  2074,  2077,  2080,  2083,
    2093,  2094,  2095,  2096,  2097,  2098,  2099,  2100,  2101,  2102,
    2103,  2104,  2105,  2106,  2107,  2108,  2114,  2118,  2120,  2121,
    2122,  2123,  2124,  2125,  2126,  2127,  2128,  2129,  2133,  2134,
    2135,  2138,  2143,  2147,  2150,  2153,  2155,  2159,  2163,  2167,
    2170,  2173,  2174,  2175,  2176,  2177,  2178,  2179,  2180,  2183,
    2189,  2192,  2195,  2199,  2202,  2206,  2210,  2214,  2218,  2222,
    2225,  2229,  2232,  2247,  2250,  2253,  2256,  2260,  2264,  2267,
    2271,  2275,  2286,  2287,  2288,  2289,  2290,  2291,  2292,  2293,
    2294,  2295,  2296,  2297,  2298,  2299,  2300,  2301,  2302,  2303,
    2304,  2305,  2306,  2307,  2308,  2309,  2310,  2311,  2312,  2313,
    2314,  2315,  2316,  2317,  2318,  2319,  2320,  2321,  2322,  2323,
    2324,  2325,  2326,  2327,  2328,  2329,  2330,  2331,  2332,  2333,
    2334,  2335,  2336,  2337,  2338,  2339,  2340,  2341,  2342,  2343,
    2344,  2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,  2353,
    2354,  2355,  2356,  2357,  2358,  2359,  2360,  2361,  2362,  2363,
    2364,  2365,  2366,  2367,  2368,  2369,  2370,  2371,  2372,  2373,
    2374,  2375,  2376,  2377,  2378,  2379,  2380,  2381,  2382,  2383,
    2384,  2385,  2386,  2387,  2388,  2389,  2390,  2391,  2392,  2393,
    2394,  2395,  2396,  2397,  2398,  2399,  2400,  2401,  2402,  2403,
    2404,  2405,  2406,  2407,  2408,  2409,  2410,  2411,  2412,  2413,
    2414,  2415,  2416,  2417,  2418,  2419,  2420,  2421,  2422,  2423,
    2424,  2425,  2426,  2431,  2438,  2438,  2469,  2496,  2506,  2552,
    2583,  2605,  2628,  2652,  2663,  2674,  2683,  2697,  2708,  2708,
    2735,  2735,  2746,  2747,  2748,  2749,  2750,  2751,  2754,  2761,
    2764,  2770,  2775,  2783,  2786,  2792,  2795,  2801,  2804,  2810,
    2815,  2823,  2826,  2834,  2840,  2845,  2848,  2858,  2864,  2870,
    2874,  2880,  2885,  2892,  2895,  2904,  2913,  2923,  2934,  2944,
    2955,  2966,  2981,  2989,  2992,  2998,  3004,  3010,  3016,  3021,
    3027,  3033,  3042,  3053,  3065,  3074,  3086,  3108,  3128,  3137,
    3147,  3152,  3160,  3166,  3169,  3172,  3178,  3190,  3202,  3206,
    3209,  3218,  3225,  3230,  3234,  3240,  3248,  3251,  3257,  3260,
    3266,  3269,  3276,  3279,  3286,  3290,  3295,  3300,  3303,  3306,
    3311,  3314,  3317,  3323,  3326,  3336,  3339,  3346,  3349,  3356,
    3359,  3366,  3371,  3378,  3384,  3390,  3394,  3401,  3405,  3412,
    3416,  3423,  3426,  3430,  3436,  3439,  3443,  3449,  3452,  3456,
    3461,  3469,  3475,  3481,  3487,  3492,  3495,  3500,  3509,  3515,
    3519,  3525,  3529,  3537,  3544,  3550,  3555,  3559,  3564,  3569,
    3575,  3580,  3586,  3591,  3596,  3600,  3605,  3609,  3614,  3619,
    3624,  3629,  3634,  3640,  3646,  3652,  3657,  3663,  3669,  3675,
    3680,  3686,  3692,  3698,  3703,  3709,  3715,  3724,  3729,  3734,
    3739,  3744,  3749,  3754,  3759,  3766,  3772,  3776,  3781,  3787,
    3793,  3799,  3805,  3813,  3819,  3826,  3832,  3838,  3844,  3850,
    3855,  3860,  3865,  3873,  3878,  3882,  3886,  3893,  3899,  3905,
    3911,  3917,  3922,  3928,  3932,  3938,  3943,  3949,  3956,  3961,
    3967,  3972,  3976,  3982,  3987,  3993,  3998,  4004,  4009,  4015,
    4019,  4024,  4030,  4036,  4042,  4046,  4052,  4058,  4064,  4070,
    4085,  4091,  4097,  4113,  4129,  4145,  4161,  4177,  4193,  4199,
    4205,  4211,  4217,  4261,  4300,  4328,  4352,  4358,  4361,  4368,
    4374,  4380,  4384,  4391,  4395,  4402,  4405,  4412,  4416,  4420,
    4424,  4431,  4436,  4440,  4444,  4451,  4454,  4461,  4468,  4471,
    4478,  4484,  4490,  4493,  4500
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "B256ABS", "B256ACOS", "B256ADD1",
  "B256ADDEQUAL", "B256ALERT", "B256AMP", "B256AMPEQUAL", "B256AND",
  "B256ARC", "B256ARRAYBASE", "B256ASC", "B256ASIN", "B256ASSIGNED",
  "B256ATAN", "B256BEGINCASE", "B256BINARYNOT", "B256BINARYOR",
  "B256BITSHIFTL", "B256BITSHIFTR", "B256BLACK", "B256BLUE", "B256BOOLEOF",
  "B256BOOLFALSE", "B256BOOLTRUE", "B256CALL", "B256CASE", "B256CATCH",
  "B256CEIL", "B256CHANGEDIR", "B256CHORD", "B256CHR", "B256CIRCLE",
  "B256CLEAR", "B256CLG", "B256CLICKB", "B256CLICKCLEAR", "B256CLICKX",
  "B256CLICKY", "B256CLOSE", "B256CLS", "B256CONFIRM", "B256CONTINUEDO",
  "B256CONTINUEFOR", "B256CONTINUEWHILE", "B256COS", "B256COUNT",
  "B256COUNTX", "B256CURRENTDIR", "B256CYAN", "B256DARKBLUE",
  "B256DARKCYAN", "B256DARKGREEN", "B256DARKGREY", "B256DARKORANGE",
  "B256DARKPURPLE", "B256DARKRED", "B256DARKYELLOW", "B256DAY",
  "B256DBCLOSE", "B256DBCLOSESET", "B256DBEXECUTE", "B256DBFLOAT",
  "B256DBINT", "B256DBNULL", "B256DBOPEN", "B256DBOPENSET", "B256DBROW",
  "B256DBSTRING", "B256DEBUGINFO", "B256DEGREES", "B256DIM", "B256DIR",
  "B256DIVEQUAL", "B256DO", "B256EDITVISIBLE", "B256ELLIPSE", "B256ELSE",
  "B256END", "B256ENDCASE", "B256ENDFUNCTION", "B256ENDIF",
  "B256ENDSUBROUTINE", "B256ENDTRY", "B256ENDWHILE",
  "B256ERROR_ARGUMENTCOUNT", "B256ERROR_ARRAYELEMENT",
  "B256ERROR_ARRAYEVEN", "B256ERROR_ARRAYEXPR", "B256ERROR_ARRAYINDEX",
  "B256ERROR_ARRAYINDEXMISSING", "B256ERROR_ARRAYLENGTH2D",
  "B256ERROR_ARRAYNITEMS", "B256ERROR_ARRAYSIZELARGE",
  "B256ERROR_ARRAYSIZESMALL", "B256ERROR_ASINACOSRANGE",
  "B256ERROR_BOOLEANCONV", "B256ERROR_DBCOLNO", "B256ERROR_DBCONNNUMBER",
  "B256ERROR_DBNOTOPEN", "B256ERROR_DBNOTSET", "B256ERROR_DBNOTSETROW",
  "B256ERROR_DBOPEN", "B256ERROR_DBQUERY", "B256ERROR_DBSETNUMBER",
  "B256ERROR_DIVZERO", "B256ERROR_DOWNLOAD", "B256ERROR_ENVELOPEMAX",
  "B256ERROR_ENVELOPEODD", "B256ERROR_EXPECTEDARRAY",
  "B256ERROR_EXPECTEDSOUND", "B256ERROR_FILENOTOPEN",
  "B256ERROR_FILENUMBER", "B256ERROR_FILEOPEN", "B256ERROR_FILEOPERATION",
  "B256ERROR_FILERESET", "B256ERROR_FILEWRITE", "B256ERROR_FOLDER",
  "B256ERROR_FREEDB", "B256ERROR_FREEDBSET", "B256ERROR_FREEFILE",
  "B256ERROR_FREENET", "B256ERROR_HARMONICLIST",
  "B256ERROR_HARMONICNUMBER", "B256ERROR_IMAGEFILE",
  "B256ERROR_IMAGERESOURCE", "B256ERROR_IMAGESAVETYPE",
  "B256ERROR_IMAGESCALE", "B256ERROR_INFINITY", "B256ERROR_INTEGERRANGE",
  "B256ERROR_INVALIDKEYNAME", "B256ERROR_INVALIDPROGNAME",
  "B256ERROR_INVALIDRESOURCE", "B256ERROR_LOGRANGE", "B256ERROR_LONGRANGE",
  "B256ERROR_MAXRECURSE", "B256ERROR_NETACCEPT", "B256ERROR_NETBIND",
  "B256ERROR_NETCONN", "B256ERROR_NETHOST", "B256ERROR_NETNONE",
  "B256ERROR_NETREAD", "B256ERROR_NETSOCK", "B256ERROR_NETSOCKNUMBER",
  "B256ERROR_NETSOCKOPT", "B256ERROR_NETWRITE", "B256ERROR_NEXTNOFOR",
  "B256ERROR_NONE", "B256ERROR_NOSUCHFUNCTION", "B256ERROR_NOSUCHLABEL",
  "B256ERROR_NOSUCHSUBROUTINE", "B256ERROR_NOTARRAY",
  "B256ERROR_NOTIMPLEMENTED", "B256ERROR_NUMBERCONV",
  "B256ERROR_NUMBEREXPR", "B256ERROR_ONEDIMENSIONAL",
  "B256ERROR_ONERRORSUB", "B256ERROR_PENWIDTH", "B256ERROR_PERMISSION",
  "B256ERROR_POLYPOINTS", "B256ERROR_PRINTERNOTOFF",
  "B256ERROR_PRINTERNOTON", "B256ERROR_PRINTEROPEN", "B256ERROR_RADIX",
  "B256ERROR_RADIXSTRING", "B256ERROR_REFNOTASSIGNED", "B256ERROR_RGB",
  "B256ERROR_SERIALPARAMETER", "B256ERROR_SETTINGMAXKEYS",
  "B256ERROR_SETTINGMAXLEN", "B256ERROR_SETTINGSGETACCESS",
  "B256ERROR_SETTINGSSETACCESS", "B256ERROR_SLICESIZE",
  "B256ERROR_SOUNDERROR", "B256ERROR_SOUNDFILE",
  "B256ERROR_SOUNDFILEFORMAT", "B256ERROR_SOUNDLENGTH",
  "B256ERROR_SOUNDNOTSEEKABLE", "B256ERROR_SOUNDRESOURCE",
  "B256ERROR_SPRITENA", "B256ERROR_SPRITENUMBER", "B256ERROR_SPRITESLICE",
  "B256ERROR_SQRRANGE", "B256ERROR_STACKUNDERFLOW",
  "B256ERROR_STRING2NOTE", "B256ERROR_STRINGCONV", "B256ERROR_STRINGEXPR",
  "B256ERROR_STRINGMAXLEN", "B256ERROR_STRSTART",
  "B256ERROR_TOOMANYSOUNDS", "B256ERROR_UNEXPECTEDRETURN",
  "B256ERROR_UNSERIALIZEFORMAT", "B256ERROR_VARCIRCULAR",
  "B256ERROR_VARNOTASSIGNED", "B256ERROR_VARNULL",
  "B256ERROR_WAVEFORMLOGICAL", "B256ERROR_WAVOBSOLETE", "B256EXISTS",
  "B256EXITDO", "B256EXITFOR", "B256EXITWHILE", "B256EXP", "B256EXPLODE",
  "B256EXPLODEX", "B256FASTGRAPHICS", "B256FILL", "B256FLOOR", "B256FONT",
  "B256FOR", "B256FOREACH", "B256FREEDB", "B256FREEDBSET", "B256FREEFILE",
  "B256FREENET", "B256FROMBINARY", "B256FROMHEX", "B256FROMOCTAL",
  "B256FROMRADIX", "B256FUNCTION", "B256GETBRUSHCOLOR", "B256GETCOLOR",
  "B256GETPENWIDTH", "B256GETSETTING", "B256GETSLICE", "B256GLOBAL",
  "B256GOSUB", "B256GOTO", "B256GRAPHHEIGHT", "B256GRAPHSIZE",
  "B256GRAPHVISIBLE", "B256GRAPHWIDTH", "B256GREEN", "B256GREY", "B256GTE",
  "B256HOUR", "B256IF", "B256IN", "B256IMAGEAUTOCROP", "B256IMAGECENTERED",
  "B256IMAGECOPY", "B256IMAGECROP", "B256IMAGEDRAW", "B256IMAGEFLIP",
  "B256IMAGEHEIGHT", "B256IMAGELOAD", "B256IMAGENEW", "B256IMAGEPIXEL",
  "B256IMAGERESIZE", "B256IMAGEROTATE", "B256IMAGESETPIXEL",
  "B256IMAGESMOOTH", "B256IMAGETRANSFORMED", "B256IMAGETYPE_BMP",
  "B256IMAGETYPE_JPG", "B256IMAGETYPE_PNG", "B256IMAGEWIDTH",
  "B256IMGLOAD", "B256IMGSAVE", "B256IMPLODE", "B256INPUT",
  "B256INPUTFLOAT", "B256INPUTINT", "B256INPUTSTRING", "B256INSTR",
  "B256INSTRX", "B256INTDIV", "B256ISNUMERIC", "B256KEY", "B256KEYPRESSED",
  "B256KILL", "B256LASTERROR", "B256LASTERROREXTRA", "B256LASTERRORLINE",
  "B256LASTERRORMESSAGE", "B256LEFT", "B256LENGTH", "B256LET", "B256LINE",
  "B256LJUST", "B256LOG", "B256LOGTEN", "B256LOWER", "B256LTE",
  "B256LTRIM", "B256MAP", "B256MD5", "B256MID", "B256MIDX", "B256MINUTE",
  "B256MOD", "B256MONTH", "B256MOUSEB", "B256MOUSEBUTTON_CENTER",
  "B256MOUSEBUTTON_DOUBLECLICK", "B256MOUSEBUTTON_LEFT",
  "B256MOUSEBUTTON_NONE", "B256MOUSEBUTTON_RIGHT", "B256MOUSEX",
  "B256MOUSEY", "B256MSEC", "B256MULEQUAL", "B256NE", "B256NETADDRESS",
  "B256NETCLOSE", "B256NETCONNECT", "B256NETDATA", "B256NETLISTEN",
  "B256NETREAD", "B256NETWRITE", "B256NEXT", "B256NOT", "B256OFFERROR",
  "B256ONERROR", "B256OPEN", "B256OPENB", "B256OPENSERIAL", "B256OR",
  "B256ORANGE", "B256OSTYPE", "B256OSTYPE_ANDROID", "B256OSTYPE_LINUX",
  "B256OSTYPE_MACINTOSH", "B256OSTYPE_WINDOWS", "B256OUTPUTVISIBLE",
  "B256PAUSE", "B256PENWIDTH", "B256PI", "B256PIE", "B256PIXEL",
  "B256PLOT", "B256POLY", "B256PORTIN", "B256PORTOUT", "B256PRINT",
  "B256PRINTERCANCEL", "B256PRINTEROFF", "B256PRINTERON",
  "B256PRINTERPAGE", "B256PROMPT", "B256PURPLE", "B256PUTSLICE",
  "B256RADIANS", "B256RAND", "B256READ", "B256READBYTE", "B256READLINE",
  "B256RECT", "B256RED", "B256REDIM", "B256REF", "B256REFRESH",
  "B256REGEXMINIMAL", "B256REM", "B256REPLACE", "B256REPLACEX",
  "B256RESET", "B256RETURN", "B256RGB", "B256RIGHT", "B256RJUST",
  "B256ROUND", "B256RTRIM", "B256SAY", "B256SECOND", "B256SEED",
  "B256SEEK", "B256SEMICOLON", "B256SEMICOLONEQUAL", "B256SERIALIZE",
  "B256SETCOLOR", "B256SETGRAPH", "B256SETSETTING", "B256SIN", "B256SIZE",
  "B256SLICE_ALL", "B256SLICE_PAINT", "B256SLICE_SPRITE", "B256SOUND",
  "B256SOUNDENVELOPE", "B256SOUNDFADE", "B256SOUNDHARMONICS",
  "B256SOUNDID", "B256SOUNDLENGTH", "B256SOUNDLOAD", "B256SOUNDLOADRAW",
  "B256SOUNDLOOP", "B256SOUNDPAUSE", "B256SOUNDPLAY", "B256SOUNDPLAYER",
  "B256SOUNDPLAYEROFF", "B256SOUNDPOSITION", "B256SOUNDSAMPLERATE",
  "B256SOUNDSEEK", "B256SOUNDSTATE", "B256SOUNDSTOP", "B256SOUNDSYSTEM",
  "B256SOUNDVOLUME", "B256SOUNDWAIT", "B256SOUNDWAVEFORM",
  "B256SPRITECOLLIDE", "B256SPRITEDIM", "B256SPRITEH", "B256SPRITEHIDE",
  "B256SPRITELOAD", "B256SPRITEMOVE", "B256SPRITEO", "B256SPRITEPLACE",
  "B256SPRITEPOLY", "B256SPRITER", "B256SPRITES", "B256SPRITESHOW",
  "B256SPRITESLICE", "B256SPRITEV", "B256SPRITEW", "B256SPRITEX",
  "B256SPRITEY", "B256SQR", "B256STAMP", "B256STEP", "B256SUB1",
  "B256SUBEQUAL", "B256SUBROUTINE", "B256SYSTEM", "B256TAN", "B256TEXT",
  "B256TEXTHEIGHT", "B256TEXTWIDTH", "B256THEN", "B256THROWERROR",
  "B256TO", "B256TOBINARY", "B256TOFLOAT", "B256TOHEX", "B256TOINT",
  "B256TOOCTAL", "B256TORADIX", "B256TOSTRING", "B256TRIM", "B256TRY",
  "B256TYPEOF", "B256TYPE_ARRAY", "B256TYPE_FLOAT", "B256TYPE_INT",
  "B256TYPE_MAP", "B256TYPE_REF", "B256TYPE_STRING", "B256TYPE_UNASSIGNED",
  "B256UNASSIGN", "B256UNLOAD", "B256UNSERIALIZE", "B256UNTIL",
  "B256UPPER", "B256VARIABLEWATCH", "B256VERSION", "B256VOLUME",
  "B256WARNING_ARRAYELEMENT", "B256WARNING_BOOLEANCONV",
  "B256WARNING_INTEGERRANGE", "B256WARNING_LONGRANGE",
  "B256WARNING_NUMBERCONV", "B256WARNING_REFNOTASSIGNED",
  "B256WARNING_SOUNDERROR", "B256WARNING_SOUNDFILEFORMAT",
  "B256WARNING_SOUNDLENGTH", "B256WARNING_SOUNDNOTSEEKABLE",
  "B256WARNING_START", "B256WARNING_STRING2NOTE", "B256WARNING_STRINGCONV",
  "B256WARNING_VARNOTASSIGNED", "B256WARNING_WAVOBSOLETE", "B256WAVLENGTH",
  "B256WAVPAUSE", "B256WAVPLAY", "B256WAVPOS", "B256WAVSEEK",
  "B256WAVSTATE", "B256WAVSTOP", "B256WAVWAIT", "B256WHILE", "B256WHITE",
  "B256WRITE", "B256WRITEBYTE", "B256WRITELINE", "B256XOR", "B256YEAR",
  "B256YELLOW", "B256ZFILL", "B256INTEGER", "B256FLOAT", "B256STRING",
  "B256HEXCONST", "B256BINCONST", "B256OCTCONST", "B256VARIABLE",
  "B256LABEL", "','", "'<'", "'>'", "'='", "'-'", "'+'", "'*'", "'/'",
  "B256UNARY", "'^'", "'['", "']'", "'{'", "'}'", "'('", "')'", "'\\n'",
  "':'", "'?'", "'%'", "$accept", "program", "programnewline",
  "programline", "label", "compoundstmt", "array_none", "array_size",
  "array_size_rows", "array_size_cols", "array_indexing", "variable",
  "variable_a", "array_element", "functionvariable",
  "functionvariablelist", "functionvariables", "listoflists", "listinlist",
  "listofitems", "listitems", "listofmapitems", "mapitems", "callexprlist",
  "callexpr", "args_none", "args_ee", "args_ea", "args_ev", "args_eee",
  "args_eeee", "args_eeeee", "args_eeeeee", "args_eeeeeee",
  "args_eeeeeeeee", "args_eeeeeeeeee", "expr", "expr_multi",
  "expr_function", "expr_constants", "expr_errors", "expr_numeric",
  "expr_string", "expr_dataelement", "statement", "begincasestmt",
  "casestmt", "$@1", "catchstmt", "dostmt", "elsestmt", "endcasestmt",
  "endifstmt", "endtrystmt", "endwhilestmt", "ifstmt", "ifthenstmt",
  "ifthenelsestmt", "ifthenelse", "trystmt", "untilstmt", "$@2",
  "whilestmt", "$@3", "letstmt", "mapstmt", "dimstmt", "redimstmt",
  "pausestmt", "throwerrorstmt", "clearstmt", "fastgraphicsstmt",
  "graphsizestmt", "refreshstmt", "endstmt", "arrayelementassign",
  "arrayassign", "assign", "forstmt", "foreachstmt", "nextstmt",
  "gotostmt", "gosubstmt", "callstmt", "offerrorstmt", "onerrorstmt",
  "returnstmt", "colorstmt", "soundstmt", "soundplaystmt",
  "soundpausestmt", "soundplayeroffstmt", "soundstopstmt", "soundwaitstmt",
  "soundwaveformstmt", "soundsystemstmt", "soundenvelopestmt",
  "soundharmonicsstmt", "soundfadestmt", "soundseekstmt",
  "soundvolumestmt", "soundloopstmt", "plotstmt", "linestmt", "circlestmt",
  "ellipsestmt", "arcstmt", "chordstmt", "piestmt", "rectstmt", "textstmt",
  "fontstmt", "saystmt", "systemstmt", "volumestmt", "polystmt",
  "stampstmt", "openstmt", "writestmt", "writelinestmt", "writebytestmt",
  "closestmt", "resetstmt", "seedstmt", "seekstmt", "inputstmt",
  "printstmt", "wavpausestmt", "wavplaystmt", "wavseekstmt", "wavstopstmt",
  "wavwaitstmt", "putslicestmt", "imgloadstmt", "spritedimstmt",
  "spriteloadstmt", "spriteslicestmt", "spritepolystmt", "spriteplacestmt",
  "spritemovestmt", "spritehidestmt", "spriteshowstmt", "clickclearstmt",
  "changedirstmt", "dbopenstmt", "dbclosestmt", "dbexecutestmt",
  "dbopensetstmt", "dbclosesetstmt", "netlistenstmt", "netconnectstmt",
  "netwritestmt", "netclosestmt", "killstmt", "setsettingstmt",
  "portoutstmt", "imgsavestmt", "editvisiblestmt", "graphvisiblestmt",
  "outputvisiblestmt", "globalstmt", "penwidthstmt", "alertstmt",
  "continuedostmt", "continueforstmt", "continuewhilestmt", "exitdostmt",
  "exitforstmt", "exitwhilestmt", "printercancelstmt", "printeroffstmt",
  "printeronstmt", "printerpagestmt", "functionstmt", "subroutinestmt",
  "endfunctionstmt", "endsubroutinestmt", "regexminimalstmt",
  "unassignstmt", "variablewatchstmt", "imagecropstmt",
  "imageautocropstmt", "imageresizestmt", "imagesetpixelstmt",
  "imagedrawstmt", "imagecenteredstmt", "imagetransformedstmt",
  "imagerotatestmt", "imageflipstmt", "imagesmoothstmt", "unloadstmt",
  "setgraphstmt", "arraybasestmt", 0
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
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
     685,   686,   687,   688,   689,   690,   691,   692,   693,   694,
     695,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   712,   713,   714,
     715,   716,   717,   718,   719,   720,   721,   722,   723,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,    44,    60,    62,
      61,    45,    43,    42,    47,   752,    94,    91,    93,   123,
     125,    40,    41,    10,    58,    63,    37
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   517,   518,   518,   519,   520,   520,   520,   520,   520,
     520,   520,   520,   521,   522,   522,   523,   524,   525,   525,
     526,   526,   527,   527,   527,   528,   528,   529,   529,   529,
     530,   530,   531,   532,   532,   533,   533,   534,   535,   535,
     535,   536,   537,   537,   538,   539,   539,   540,   540,   541,
     541,   542,   542,   543,   543,   544,   544,   545,   545,   546,
     546,   547,   547,   548,   548,   549,   549,   550,   550,   551,
     551,   552,   552,   553,   553,   553,   553,   553,   553,   553,
     553,   554,   554,   554,   554,   554,   555,   555,   556,   556,
     556,   556,   556,   556,   556,   556,   556,   556,   556,   556,
     556,   556,   556,   556,   556,   556,   556,   556,   556,   556,
     556,   556,   556,   556,   556,   556,   556,   556,   556,   556,
     556,   556,   556,   556,   556,   556,   556,   556,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   557,   557,   557,   557,   557,
     557,   557,   557,   557,   557,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   559,   559,   559,   559,   559,   559,   559,
     559,   559,   559,   560,   560,   560,   560,   560,   560,   560,
     560,   560,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   562,   564,   563,   565,   566,   567,   568,
     569,   570,   571,   572,   573,   574,   575,   576,   578,   577,
     580,   579,   581,   581,   581,   581,   581,   581,   582,   583,
     583,   583,   583,   583,   583,   583,   583,   584,   584,   584,
     584,   584,   584,   585,   586,   587,   587,   587,   588,   589,
     589,   590,   591,   592,   592,   592,   592,   592,   592,   592,
     592,   592,   593,   594,   594,   594,   594,   594,   594,   594,
     594,   594,   595,   595,   596,   596,   597,   597,   598,   599,
     600,   600,   601,   602,   602,   602,   603,   603,   604,   604,
     604,   605,   605,   606,   606,   606,   607,   607,   608,   608,
     609,   609,   610,   610,   611,   611,   612,   613,   613,   613,
     614,   614,   614,   615,   615,   616,   616,   617,   617,   618,
     618,   619,   620,   621,   622,   623,   623,   624,   624,   625,
     625,   626,   626,   626,   627,   627,   627,   628,   628,   628,
     628,   629,   630,   631,   632,   633,   633,   633,   634,   634,
     634,   634,   634,   634,   634,   634,   634,   634,   635,   635,
     636,   636,   637,   637,   638,   638,   639,   639,   640,   641,
     641,   642,   642,   642,   642,   642,   642,   642,   642,   642,
     642,   642,   642,   642,   642,   642,   642,   643,   643,   643,
     643,   643,   643,   643,   643,   644,   645,   645,   646,   647,
     648,   649,   650,   650,   650,   651,   652,   653,   654,   655,
     655,   655,   655,   656,   656,   656,   656,   657,   658,   659,
     660,   661,   661,   662,   662,   663,   663,   664,   664,   664,
     665,   665,   665,   666,   666,   667,   667,   668,   668,   669,
     669,   670,   671,   672,   673,   673,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   687,
     688,   689,   690,   691,   692,   693,   694,   695,   695,   696,
     697,   698,   698,   699,   699,   700,   700,   701,   701,   701,
     701,   702,   702,   702,   702,   703,   703,   704,   705,   705,
     706,   707,   708,   708,   709
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     1,     1,     2,     3,     1,     2,     1,
       2,     1,     0,     1,     3,     1,     2,     3,     4,     2,
       4,     2,     5,     6,     3,     1,     3,     1,     2,     3,
       2,     3,     1,     2,     3,     1,     3,     3,     1,     1,
       3,     3,     1,     3,     3,     4,     6,     1,     3,     1,
       4,     2,     0,     3,     3,     3,     3,     3,     3,     5,
       3,     7,     3,     9,     3,    11,     3,    13,     3,    17,
       3,    19,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     4,     3,     2,     2,
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
       2,     2,     2,     2,     2,     1,     1,     2,     2,     2,
       3,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     2,     4,
       4,     4,     4,     6,     8,    10,     6,     8,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     2,     2,     2,     4,     4,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     4,     2,     4,
       2,     2,     2,     2,     2,     2,     2,     6,     8,    10,
       2,     2,     2,     8,     6,     4,     4,     4,     4,     4,
       4,     4,     4,     2,     4,     6,     4,     6,     8,     4,
       6,     8,     4,     6,     8,     2,     2,     2,     4,     4,
       6,     8,     6,     2,     2,     4,     6,     2,     6,     2,
       4,     2,     2,     4,     2,     2,     2,     4,     6,     4,
       4,     4,     6,     1,     1,     1,     2,     2,     2,     4,
       4,     4,     2,     4,     2,     4,     2,     4,     2,     2,
       4,     4,     4,     4,     4,     4,     4,     2,     2,     2,
       1,     4,     4,     4,     4,     8,     6,     8,     6,     6,
       2,     4,     2,     4,     2,     4,     6,     8,     2,     2,
       2,     4,     2,     4,     6,     4,     2,     8,    10,     8,
       4,     4,     4,     4,     4,     6,     4,     4,     4,     6,
       4,     4,     4,     4,     4,     2,     2,     2,     4,     4,
       4,     8,     6,     4,    12,    10,     4,     2,     4,     4,
       4,     4,     4,     1,     1,     1,     4,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     2,     3,     1,     0,     3,
       0,     3,     2,     2,     2,     1,     1,     1,     2,     2,
       4,     3,     5,     3,     5,     4,     4,     2,     4,     3,
       5,     3,     5,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     3,     3,     3,     3,
       3,     3,     6,     8,     4,     7,     2,     1,     2,     2,
       4,     5,     2,     2,     4,     5,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     2,
       4,     2,     3,     2,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     3,     3,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      12,     0,     0,     0,   633,     0,   634,   636,     0,     0,
       0,    52,    52,    52,    52,   882,   883,   884,    52,    52,
       0,     0,     0,     0,   637,     0,     0,   638,    52,   639,
     894,   640,   895,   641,   642,   885,   886,   887,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,     0,   707,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    52,    52,    52,
      52,     0,     0,     0,    52,     0,    11,    52,    52,     0,
       0,     0,     0,    52,     0,     0,    52,     0,    52,     0,
      52,    52,    52,     0,    52,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   647,     0,     0,   648,     0,     0,    52,    52,
       0,    52,    52,   650,     0,     0,     0,    25,    13,     0,
      52,     0,     3,     9,     7,    27,     0,     0,    15,   495,
     497,   498,   515,   519,   520,   522,   525,   526,   540,   541,
     542,     0,   618,   621,   629,   557,   559,   514,   582,   569,
     617,   502,   530,   538,   583,   523,   657,   656,   655,   532,
     533,   564,   537,   536,   496,   565,   566,   586,   505,   591,
     594,   592,   593,   595,   596,   597,   598,   599,   600,   601,
     602,   603,   604,   572,   558,   501,   518,   493,   500,   571,
     581,   616,   531,   587,   615,   623,   573,   613,   567,   632,
     631,   630,   504,   585,   588,   589,   555,   579,   624,   625,
     626,   627,   628,   580,   553,   605,   607,   612,   610,   609,
     608,   606,   611,   503,   499,   513,   510,   511,   512,   509,
     562,   561,   563,   560,   556,   590,   574,   554,   517,   539,
     568,   535,   570,   492,   506,   507,   508,   527,   528,   529,
     575,   576,   577,   578,   534,   614,   521,   524,   584,   619,
     622,   545,   543,   549,   548,   546,   544,   552,   550,   547,
     551,   620,   516,   494,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    52,    52,    52,    52,     0,     0,    52,
      52,    52,    52,     0,     0,     0,     0,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,     0,     0,
       0,    52,     0,     0,     0,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,     0,     0,
       0,     0,     0,    52,    52,    52,    52,     0,     0,     0,
       0,    52,    52,    52,     0,     0,    52,    52,    52,    52,
      52,    52,     0,     0,     0,     0,    52,    52,    52,     0,
       0,     0,     0,     0,    52,    52,    52,    52,    52,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,     0,    52,    52,    52,    52,    52,
      52,    52,     0,     0,     0,    52,     0,    52,    52,    52,
      52,    52,     0,     0,     0,     0,     0,     0,     0,    52,
       0,     0,    52,    52,    52,    52,    52,    52,     0,     0,
       0,    52,    52,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    52,    52,
      52,    52,    52,    52,     0,     0,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,     0,   255,
     256,   420,   394,   393,   395,     0,     0,     0,     0,    27,
     483,    80,   484,   485,   881,    73,    74,    75,    76,    77,
      78,    79,     0,   755,   756,     0,   924,     0,     0,     0,
     850,   757,   758,     0,   753,     0,     0,   676,   677,     0,
     849,   794,   795,   675,   853,   854,     0,   860,   862,   861,
       0,   856,   855,   852,   851,     0,   858,   859,   857,    27,
       0,   659,   876,     0,   754,     0,   682,   678,     0,   769,
     768,   767,   770,     0,     0,     0,     0,    27,    32,    35,
     879,   709,   708,   679,   680,     0,   877,     0,   902,   901,
       0,   914,   913,   912,   911,     0,     0,   900,     0,   910,
     909,   908,   907,   919,   918,   904,   903,   917,     0,     0,
     906,   905,     0,   920,     0,   916,   915,     0,     0,   832,
     833,   834,     0,   875,   874,     0,    27,    80,   803,   801,
       0,    27,    80,   815,   813,    27,    80,   811,   809,    27,
      80,   807,   805,   871,   654,   653,   652,   752,   658,   869,
     870,   865,   866,   864,   863,   868,   867,   706,   712,   713,
     779,   778,   781,   780,     0,   782,   783,   784,   785,   786,
     787,     0,   878,   673,   880,   759,   760,   751,   774,   873,
       0,   819,   817,    42,   888,   889,   890,   891,   831,     0,
     761,   762,   763,     0,     0,   667,   681,   896,   796,   797,
     716,   717,   771,   798,   800,   799,   719,   720,   718,   923,
     922,   872,   721,   722,     0,   737,   738,   739,   744,   743,
     740,   741,   742,   749,   750,   727,   726,   723,   724,   725,
     729,   728,   745,   746,   731,   730,   736,   747,   748,   733,
     732,   735,   734,   835,   847,   836,   843,   844,   845,   846,
     839,   840,   841,   842,   838,   848,   837,   777,   776,   775,
       0,   772,     0,   764,   765,   766,     0,   674,   897,   898,
     921,     0,   899,   773,   825,   826,   827,   828,   829,   830,
       0,   789,   788,   793,   792,   791,   790,     0,     0,     0,
       0,   823,   821,    42,     1,     4,    12,    10,     5,     8,
     694,     0,     0,     0,     0,     0,   695,     0,     0,    28,
      30,     0,     0,   684,     0,     0,     0,     0,     0,   685,
       0,     0,     0,   644,   645,     0,     0,     0,   287,   283,
       0,     0,     0,     0,   270,    88,    89,     0,   315,    90,
      91,     0,     0,    92,   336,   334,   335,     0,     0,     0,
       0,   434,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   320,     0,     0,     0,     0,   353,     0,     0,     0,
       0,   446,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,     0,     0,   487,   488,   489,     0,
     381,     0,   382,   384,   385,     0,     0,     0,     0,   341,
     340,   342,     0,     0,   490,   491,     0,   325,   324,   102,
     103,   321,     0,   477,     0,     0,     0,     0,   465,   466,
     467,     0,     0,     0,     0,     0,   330,     0,   328,   365,
     439,   366,   438,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   322,   319,   333,   104,   105,   106,   107,
     108,   331,   332,   374,   442,     0,   367,     0,   440,   274,
     109,   373,   110,   111,   112,   113,   314,     0,     0,     0,
     114,     0,   313,     0,   430,     0,   379,     0,   432,   115,
       0,     0,     0,     0,     0,     0,     0,   323,     0,     0,
       0,   326,   116,   117,   118,   402,     0,   408,     0,     0,
       0,     0,   404,   409,     0,   406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   288,   284,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,   120,   121,   122,   123,   124,   125,     0,     0,
     386,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   396,   397,   398,   126,
     318,   127,     0,   259,   257,   258,     0,     0,    39,    38,
       0,    42,    27,   483,    80,     0,   285,   286,     0,     0,
      28,   417,   418,   419,   281,   282,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,   635,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,   663,   661,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   892,     0,
       0,     0,   643,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   818,     0,     0,     0,   671,
     669,     0,   893,     0,     0,   649,   651,    26,    29,    31,
       0,   822,     2,     6,   696,   700,   699,   698,   701,   697,
      16,     0,   692,   693,   686,   690,   689,   688,   691,   687,
     683,    14,   646,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    44,
       0,    81,     0,     0,     0,    87,     0,    47,    49,     0,
      21,    19,    84,   271,   267,   268,   269,   279,   264,   280,
     261,   276,   272,    83,   273,   277,   278,   275,   260,    82,
      85,   265,   266,   262,    64,    66,     0,   710,     0,    60,
       0,    54,    53,    53,   666,   665,     0,     0,   660,    62,
       0,    53,     0,   704,     0,    33,     0,    36,     0,     0,
       0,    70,    72,     0,     0,    56,    58,    57,    55,   714,
       0,    68,    53,   820,    43,    42,     0,     0,     0,   668,
       0,   824,     0,    24,   312,   304,   292,   303,   415,   416,
     305,   316,   298,   421,     0,   387,   301,     0,     0,     0,
     359,     0,   356,     0,   362,     0,   354,     0,   435,   460,
     306,   445,   317,   311,   299,   383,   389,   390,   391,     0,
       0,     0,     0,   476,   411,   473,     0,   412,   410,   452,
     453,   451,     0,     0,   461,   329,     0,   291,   478,   479,
     308,   309,   424,   462,   443,     0,     0,   368,   441,     0,
     369,     0,   454,   307,   431,   380,   433,     0,     0,     0,
       0,   480,   481,   414,   413,   463,   450,   300,   327,   407,
     469,   468,   470,   401,   400,   403,   405,     0,   347,   352,
     350,   351,   349,   348,   345,   346,   310,   302,     0,     0,
     375,   456,   290,   457,   289,   458,     0,   422,   464,   399,
     486,   423,   482,    41,    40,     0,     0,     0,    17,     0,
      86,     0,     0,     0,   711,     0,   664,   662,     0,     0,
       0,     0,    34,     0,     0,     0,     0,   715,     0,     0,
     672,   670,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    45,    20,    18,     0,    48,     0,    59,
       0,    59,   702,     0,    59,     0,     0,    59,    59,     0,
      59,    22,     0,   388,     0,   370,   372,     0,   360,     0,
     357,     0,   363,   355,     0,   436,   392,   444,     0,     0,
       0,   472,     0,   293,     0,   296,   428,     0,     0,   426,
     337,   455,     0,     0,     0,   429,     0,   344,   378,   376,
     459,     0,    50,     0,     0,     0,   705,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,    61,   703,    61,     0,     0,    61,    61,   371,   361,
     358,   364,   437,     0,   471,     0,   294,   297,   425,   427,
       0,   447,   449,     0,   338,   343,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    63,     0,    63,     0,   475,
     295,   448,   339,     0,     0,     0,     0,    65,     0,    65,
     474,     0,     0,     0,    67,     0,     0,     0,    69,     0,
      71
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   151,   916,   152,   153,   154,  1300,  1301,  1302,  1303,
     930,   629,   630,   631,   699,  1358,   700,   632,  1287,  1288,
    1289,   633,  1290,  1546,  1547,   657,  1339,   748,   749,  1335,
    1349,  1328,  1329,   790,   735,   736,   705,   635,   636,   637,
     638,   639,   640,   641,   158,   159,   160,   649,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   891,   174,   900,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1381
static const int yypact[] =
{
   20936,  8947,  9447,  8947, -1381,  -472, -1381, -1381,  8947,  9447,
    9947, 10447,  -474, 10447,  -474, -1381, -1381, -1381, 10447, 10947,
   11447, 11447, 11947,  -423, -1381,  8947, 12447, -1381,  -474, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,  -474, 12947,
    -472,  -472,  -472,  -422,  -472,  -472, 11947,  8947,  8947, 11447,
   13447, 13947, 13447, 11947, 11947, 11447, 14447,  8947, 14947, 15447,
   11447, 15947, 15947, 15947, 15947,  8947,  -423, 12447,  -422, 10447,
   11947, 11447, 11447,  -472,  -474,  -472, 11447, 11447, 16447,  8947,
    8947,  8947,  9447, 11447,  8947, 11447, 16947,  -474,  -474,  -474,
    -474,  9947, 17447,  -423,  -474,  8947, -1381, 10447, 10447,  8947,
    8947, 11447, 11947, 10447,  9947, 11447, 17947, 14447, 10947, 11447,
   10447, 10947, 10447, 11447, 10447,  8947, 11447, 10447, 11447,  8947,
    8947, 11447, 13447, 13447, 11447,  8947, 13947, 15447,  -472,  8947,
   18447,  8947, -1381,  -423,  8947, -1381,  -422,  8947,  -474, 10447,
    8947,  -474,  -474, -1381, 11447, 11447, 11447, -1381, -1381,  -423,
   18947,    41,  -468, 21426,  -289,   212,  -180,   192,  -421, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, 21884, -1381,
   -1381, 21884, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381,  -416,  -384,  -364,  -376,  -370,  -369,
    -354,  8947,  -474,  -474,  -349,  -474,  -474,  -346,  -344,  -474,
    -474,  -474,  -474,  -343,  -340,  -339,  -332,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -327,  -323,
    -322,  -318,  -316,  -312,  -311,  -308,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -307,  -306,
   11947, 11447,  -305,  -474,  -304,  -474,  -474,  -303,  -302,  -301,
    -299,  -474,  -474,  -474,  -298, 19447,  -474,  -474,  -474,  -474,
    -474,  -297,  -295,  -292,  -291,  -284,  -474,  -474,  -474,  -282,
    -281,  -279,  -278,  -277,  -474,  -276,  -474,  -474,  -474,  -474,
    -275,  -274,  -273,  -272,  -271,  -270,  -267,  -266,  -263,  -261,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -258,  -256,  8947,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -252,  -250,  -249,  -474,  -243,  -474,  -242,  -240,
    -239,  -474,  -229,  -228,  -227,  -222,  -221,  -220,  -216,  -474,
    -215,  -214,  -213,  -474,  -474,  -474,  -474,  -212,  -211,  -209,
    -205,  -204,  -474,  -202,  -198,  -196,  -194,  -193,  -189,  -188,
    -186,  -185,  -182,  -179,  -364,  -177,  -175,  -174,  -173,  -166,
    -165,  -161,  -159,  -158,  -157,  -156,  -155,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -154,  -151,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,
    -474,  -474,  -474,  -474,  -474,  -474,  -474,  -474,  -149, -1381,
   -1381, -1381, -1381, -1381, -1381,  8947,  -446, 19947,  8947,    23,
   -1381,    11, -1381, -1381, 34581, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381,  9447, -1381, -1381, 23934, 34581,  -472,  -148,  8947,
   34581, -1381, -1381,  9947, -1381, 23956,  5447, -1381, 34581,  -181,
   -1381, -1381, 34581, -1381, -1381, 34581,  5947, -1381, -1381, 23976,
   11447, -1381, 23976, -1381, 23976, 11947, -1381, -1381, 24071,  -143,
    6947,    96, 34581, 12447, -1381, 24214, -1381, -1381, 12947, -1381,
   -1381, -1381, 24234,  -133,  -208,  -140,  -422,  -138, -1381,  -125,
   -1381, -1381, -1381, -1381, -1381, 24071, 34581, 24276, -1381, 23976,
   13447, -1381, -1381, -1381, -1381, 24322, 13947, -1381, 24357, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, 23976, 14447,
   -1381, -1381, 24383, 34581, 14947, -1381, -1381, 24609, 15447, -1381,
   -1381, -1381, 24635, -1381, 23976, 15947,    69,   181, -1381, -1381,
   24661,    87,   185, -1381, -1381,   132,   187, -1381, -1381,   164,
     191, -1381, -1381, 34581, -1381, -1381, -1381, -1381, -1381, -1381,
   34581, -1381, -1381, -1381, 23976, -1381, 23976, -1381, -1381,  -135,
   -1381, 23976, -1381, 23976, 16447, -1381, -1381, -1381, -1381, -1381,
   -1381, 24683, 34581, 34581, 34581, -1381, -1381, -1381, 34581, -1381,
    5447, -1381, -1381, 24735, -1381, -1381, -1381, -1381, -1381, 17447,
   -1381, -1381, -1381, 24775, 11447,   166, -1381, 34581, -1381, 34581,
   -1381, 34581, 34581, 34581, -1381, 23976, -1381, -1381, 24071, -1381,
   34581, -1381, -1381, 23976,  6447, -1381, -1381, 24214, -1381, -1381,
   -1381, -1381, 23976, -1381, 23976, -1381, 34581, -1381, -1381, 23976,
   -1381, 34581, -1381, 23976, -1381, 34581, 34581, -1381, 23976, -1381,
   34581, -1381, 23976, 34581, 34581, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, 34581, -1381, -1381, -1381, -1381,
    -140, 34581, 18447, -1381, -1381, -1381, 24887, 34581, -1381, -1381,
   34581,  8947, -1381, 34581, -1381, -1381, 34581, 34581, -1381, -1381,
    8947, -1381, 23976, -1381, 23976, -1381, 23976,  -353,  -132,  -131,
    5447, -1381, -1381, 24912, -1381, -1381, 20936, -1381,    28, -1381,
   -1381,  8947,  8947,  8947,  8947,  8947, -1381,  8947,  7447, -1381,
   -1381,  8947,  8947, -1381,  8947,  8947,  8947,  8947,  8947, -1381,
    8947,  8947, 21884,   304, -1381,  8947,  8947,  -364,  -118, -1381,
    8947,  8947,  -423,  8947,  -366, -1381, -1381,  5447, -1381, -1381,
   -1381,  8947,  8947, -1381, -1381, -1381, -1381,  8947,  8947,  8947,
    8947, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381,  8947,  8947,  8947,  5447, -1381,  8947,  8947,  8947,
    5447, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381,  8947,  8947, -1381, -1381, -1381,  8947,
   -1381,  5447, -1381, -1381, -1381,  8947,  8947,  8947,  8947, -1381,
   -1381, -1381,  8947, 19447, -1381, -1381, 24979, -1381, -1381, -1381,
   -1381, -1381,  5447, -1381,  8947,  8947,  8947,  9947, -1381, -1381,
   -1381,  8947, 11947,  8947,  8947,  8947, -1381,  5447, -1381, -1381,
   -1381, -1381, -1381,  8947,  8947, 11947,  8947,  8947,  8947,  8947,
    8947,  8947,  8947, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381,  5447, -1381,  5447, -1381,  2150,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381,  8947,  8947,  8947,
   -1381,  8947, -1381,  5447, -1381,  5447, -1381,  5447, -1381, -1381,
    8947,  8947,  8947,  8947, 11947, 11447,  8947, -1381,  8947,  8947,
    5447, -1381, -1381, -1381, -1381, -1381,  5447, -1381, 11447,  8947,
   11447,  5447, -1381, -1381,  5447, -1381,  8947,  8947,  8947,  8947,
    8947,  8947,  8947,  8947,  8947,  8947,  -118, -1381,  8947,  5447,
   -1381,  8947,  8947,  8947,  8947,  8947,  8947,  8947,  8947,  8947,
    8947, -1381, -1381, -1381, -1381, -1381, -1381, -1381,  8947,  8947,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, 11447,  -366, -1381, -1381, 19947,  -119,  -105, -1381,
    -117, 25026,     0,  -132,    17, 25052, -1381, -1381,  3438,  3947,
    -111,  -138, -1381, -1381, -1381, -1381,  8947,  8947,  8947,  8947,
    8947,  8947,  8947,  8947,  8947,  8947,  8947,  8947,  8947,  8947,
    8947,  8947,  8947,  8947,  8947,  8947,  8947, 20447,  -114,  -110,
     320,  8947,  -104,  4447, 34581,  -103,   358,  8947, -1381,  -102,
     507,  8947,  1452,  8947,  8947,  8947,   194,  1694,  8947,  -101,
    1791,  8947,  1984,  8947,  8947,   -52,   -96,  -459, -1381,  -352,
     -92,  -422, -1381,  2077,  8947,  2097,  8947,  2125,  8947,  -100,
     -88,  2236,  8947,  2489,  8947,   -87,   -86,  2509,  -364,  4947,
     -85,  2598,  8947,   -81, 21062,  8947,  8947, 21084,  8947,   213,
   22010,  8947, -1381, 22032,  8947, 34581, 34581, -1381, -1381, -1381,
     -79,  8947, -1381, -1381, 34581, 34581, 34581, 34581, 34581, 34581,
   -1381, 22110, 34581, 34581, 34581, 34581, 34581, 34581, 34581, 34581,
   34581, -1381, -1381, 25078, 25202,  -351,  8947, 25282, 25304,   -77,
     -76, 25330, 25356, 25428, 25453, 22130, 25617, 25657, 25715, 22156,
   22410, 22435, 22463, 22483, 25743, 25804, 25859, 25902, 25969, 26028,
   26048, 26089, 26175, 26254, 26333, 26362, 22535,  8947, 22555, 26405,
   26428, 26480,   -73, 26684,   -72,   -70, 22578, 26706, 26759, 26781,
   26803, 26833, 26861,   -66,   -65, 26927, 27153, 27205, 27227, 27302,
   27332, 27376, 27396, 27456, 27478, 27620, 22724, 27757, 27843, 27906,
   27929, 27995, 28015, 28035, 28081,   -64,   -63,   -61, 22851, 28135,
   28256, 28358, 28446, 28534,   -60, 22879, 28588,   -59, 22997, 28636,
   28675, 28697, 28757, 28777, 28812, 29001, 29064, 29097, 29143, 29206,
   29288, 29383, 29429, 23102, 29449, 29511, 29540, 29573, 29606, 29764,
   29846, 29884, 29956, 30076, 30135,   -58,   -55, -1381, 19947, -1381,
    7947, -1381,   -54,  -466,   -82, -1381,   -48,   -47, 34581,   -50,
   -1381, -1381,   338,  2150,   338,  -116,  -116,   257,  -366,   257,
    -366,   257,   544,  2624,   544,   257,   257,   257,  -109,  -109,
    -366,  -366,   -53,  -366, -1381, -1381, 30180, -1381,   -46, -1381,
   30217, -1381, 34581, 30217, 34581, 34581,  8947,  8947, 34581, -1381,
   30278, 30306, 30455, -1381,  -472, -1381,   -45, -1381, 30530, 30592,
   30306, -1381, -1381, 30634, 30679, -1381, -1381,  -118, -1381, -1381,
     -44, -1381, 30732, -1381, -1381, 30821, 30907,  8947,  8947, 34581,
   30970, -1381,  8947,   -38, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381,  8947, -1381, -1381,  8947,  8947,  8947,
   -1381,  8947, -1381,  8947, -1381,  8947, -1381,  8947, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,  8947,
    8947, 30993,  8947, -1381, -1381, -1381,  8947, -1381, -1381, -1381,
   -1381, -1381,  8947,  8947, -1381, -1381,  8947, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381,  8947,  8947, -1381, -1381,  8947,
   -1381,  8947, -1381, -1381, -1381, -1381, -1381,  8947,  8947,  8947,
    8947, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381,  8947, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,  8947,  8947,
   -1381, -1381, -1381, -1381, -1381, -1381,  8947, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381,  8947,   -37,   -36, -1381,  -472,
   -1381,  8447,   -35,  8947, -1381,  8947, 34581, 34581,  8947,  8947,
    8947,   237, -1381,  8947,  8947,  8947,  8947, -1381,  8947,  8947,
   34581, 34581,  8947, 31050,  8947, 31096, 23137, 31135, 23157, 23223,
   23248, 31325, 23276, 31414, 31437,  8947, 31494, 23398, 23502, 23537,
   31517, 31555, 23562, 31603, 31666, 31723, 31746, 31832, 32055, 23656,
   32075, 32110, 32133, 32171, -1381, -1381,   -34, -1381, 32196, 34581,
   32299, 32299, 32362,   -52, 32425, 32448, 32471, 32501, 32523, 32425,
   32196, -1381, 32725, -1381,  8947, -1381, -1381,  8947, -1381,  8947,
   -1381,  8947, -1381, -1381,  8947, -1381, -1381, -1381, 32501,  8947,
    8947, -1381,  8947, -1381,  8947, -1381, -1381,  8947,  8947, -1381,
   -1381, -1381,  8947,  8947,  8947, -1381,  8947, -1381, -1381, -1381,
   -1381,  8947, -1381,  8947,  8947,  8947, -1381,  8947,  8947,  8947,
    8947,  8947, -1381, 32801, 32823, 32862, 32882, 32970, 33025, 33123,
   23676, 33162, 33199, 33270, 23815, 33423, 23848, 33493, -1381, 34610,
   33526, 34581, 34581, 33526, 33564, 33584, 33564, 33805, -1381, -1381,
   -1381, -1381, -1381,  8947, -1381,  8947, -1381, -1381, -1381, -1381,
    8947, -1381, -1381,  8947, -1381, -1381,  8947,  8947,  8947,  8947,
   23914, 33844, 33864, 33890, 33923, 34581, 33949, 33969,  8947, -1381,
   -1381, -1381, -1381,  8947,  8947,  8947, 34209, 34581, 34229, 34249,
   -1381,  8947,  8947, 34275, 34581,  8947, 34304,  8947, 34555,  8947,
   34581
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1381,  -440, -1381, -1381, -1381,  -142,  -123,  -823, -1381, -1381,
   -1381,  1354,  1723,  1403, -1381,  -401,   -62, -1381, -1058, -1381,
     -83, -1381, -1380, -1293, -1381,  2823,   203,   -49,   -29,   124,
     378,   292,   283,  -296,  -251,  -247,    -1, -1381, -1381, -1381,
   -1381, -1381, -1381, -1351, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381,   416,   423,   425, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381, -1381,
   -1381, -1381, -1381
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -817
static const yytype_int16 yytable[] =
{
     634,   645,   646,   801,  1593,  1296,   768,   650,   645,   655,
     658,   918,   662,   753,   757,   761,  1304,   665,   669,   672,
     674,   678,  1304,   147,   682,   685,   943,   931,  1296,   944,
    1355,  1747,   929,   754,   758,   762,   147,   659,   692,   647,
    1578,   914,  1748,  1284,  1285,   915,   706,   707,   709,   715,
     718,   715,   696,  1595,   728,   732,   733,   737,   742,   744,
     750,   750,   750,   750,   763,   919,   685,   911,   770,  -802,
     774,   776,   147,   147,  1296,   781,   783,   791,   792,   793,
     794,   645,   728,   798,   728,   803,  1610,  -814,   149,   696,
     655,   813,  1296,   942,   817,   945,   819,   821,   822,   823,
     825,   828,   830,   655,   833,   837,   732,   842,   844,   846,
     849,   851,   853,   855,   856,   858,   860,   862,   863,   864,
     728,   715,   715,   728,   875,   718,   742,   946,   881,   886,
     887,   147,  -810,   890,   654,   950,   893,  1296,   896,   897,
    1326,   951,   952,   902,   904,   906,   677,   947,  -802,   913,
    1327,  1312,   460,   461,   928,  1360,  1426,   953,  1312,  1397,
    1397,  1397,   957,   690,  -806,   961,  -814,   962,   967,  1296,
     704,   968,   969,   475,   711,  1314,   719,   724,   726,   970,
     730,  -804,  1314,   739,   982,  -816,  1304,  -812,   983,   984,
    1304,  -808,  1304,   985,   772,   987,  1304,   933,   934,   988,
     989,   935,   786,   990,  1104,  1105,  1109,  1111,  1115,  1116,
    1117,  -810,  1118,  1122,  1132,   808,  1134,   920,   921,  1135,
    1136,   922,   668,   671,   673,   676,   827,  1137,   831,  1141,
    1142,   838,  1143,  1144,  1145,  1147,  1153,  1154,  1155,  1156,
    1157,  1158,   689,  -806,  1159,  1160,   866,   870,  1161,   703,
    1162,   877,   708,  1175,   883,  1177,   723,   725,   727,  1187,
    -804,  1188,  1189,   743,  -816,  1306,  -812,   936,  1191,  1193,
    -808,  1195,  1197,   771,   773,   775,  1308,  1309,  1310,   780,
     782,   785,  1200,  1201,  1202,   644,   797,   923,   799,  1203,
    1204,  1205,   652,  1356,   643,  1206,  1208,  1209,  1210,  1216,
    1218,   651,  1219,  1348,   824,   826,  1220,  1221,   832,  1224,
     954,   841,   843,  1226,   848,  1227,   852,  1228,  1229,   857,
     932,   861,  1230,  1231,   865,  1232,  1233,   874,  1306,  1234,
    1307,  1338,  1235,   714,  1238,   722,  1239,  1241,  1242,  1308,
    1309,  1310,   713,   717,   721,  1243,  1244,   901,   903,   905,
    1245,   741,  1246,  1247,  1248,  1249,  1250,  1258,  1309,  1310,
    1259,   789,  1282,  1333,   928,   796,  1306,  1354,  1307,  1360,
     788,  1357,  1361,  1391,   795,   812,  1379,  1308,  1309,  1310,
    1398,  1399,  1403,  1422,   811,  1322,  1323,  1324,  1325,  1426,
    1326,  1537,  1538,  1539,  1324,  1325,  1549,  1326,  1574,   594,
    1327,  1586,  1575,  1594,   684,   869,   873,  1327,  1397,  1579,
    1581,  1589,  1601,   885,   868,   872,  1410,   691,   876,   879,
    1617,  1297,   884,  -802,  1602,  1605,  1606,  1611,   712,  1749,
     720,  1613,  1305,  1621,   731,  1628,  1629,   740,  1305,  1667,
    1669,  -814,  1670,   147,  1297,   767,  1678,  1679,  1701,  1702,
    1751,  1703,  1710,  1713,  1742,  1743,   787,   627,  1807,   696,
     728,  1746,  1866,  1327,  1750,  1752,  1754,  1762,  1767,  1774,
     810,  1804,  1805,  1748,  1126,  1813,  1402,  1550,  1862,  1392,
    1744,  1888,   764,  1369,   836,   839,  -810,  1370,  1380,   765,
    1297,   766,     0,     0,   937,     0,     0,     0,     0,     0,
     867,   871,     0,     0,     0,   878,     0,  1298,  1297,     0,
       0,  1299,  1397,     0,   924,  1306,     0,  1307,  -806,     0,
       0,     0,     0,  1179,  1312,     0,  1308,  1309,  1310,  1399,
    1298,     0,     0,     0,  1299,  -804,     0,     0,     0,  -816,
       0,  -812,     0,     0,     0,  -808,     0,     0,  1314,     0,
       0,     0,  1306,  1297,  1307,  1311,   929,     0,     0,     0,
       0,   938,     0,  1308,  1309,  1310,     0,     0,     0,     0,
       0,     0,     0,     0,   929,     0,  1298,     0,     0,     0,
    1299,   925,  -802,  -802,  1107,  1297,     0,  1312,     0,     0,
       0,     0,     0,  1311,  1298,     0,     0,     0,  1299,     0,
    -814,  -814,  1305,     0,  1313,  1312,  1305,     0,  1305,     0,
       0,  1314,  1305,   939,   940,     0,     0,     0,     0,     0,
       0,     0,     0,  1315,  1283,  1312,  1291,  1295,     0,  1314,
       0,     0,     0,   926,   927,     0,     0,     0,  1316,  1298,
       0,  1330,  1313,  1299,     0,  -810,  -810,     0,  1334,  1314,
       0,     0,  1336,     0,     0,  1295,     0,     0,     0,     0,
       0,  1315,     0,  1106,  1108,  1340,     0,     0,     0,  1340,
       0,  1298,     0,     0,  1342,  1299,  1316,  -806,  -806,  1347,
       0,     0,  1350,     0,     0,     0,     0,  1352,  1317,     0,
       0,     0,   941,     0,  -804,  -804,  1375,     0,  -816,  -816,
    -812,  -812,     0,     0,  -808,  -808,     0,     0,     0,  1363,
       0,     0,     0,     0,     0,  1365,  1376,  1383,     0,   928,
       0,     0,     0,     0,     0,     0,  1317,     0,  1367,     0,
       0,     0,     0,  1371,     0,     0,     0,  1373,     0,     0,
       0,     0,  1311,     0,  1377,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,     0,  1125,     0,     0,
       0,     0,     0,  1327,  1312,     0,     0,     0,     0,  1311,
       0,     0,     0,  1381,   929,     0,     0,     0,     0,     0,
       0,  1313,     0,     0,     0,     0,     0,     0,  1314,  1384,
    1421,     0,     0,     0,     0,  1318,     0,     0,  1387,     0,
    1315,  1312,     0,  1390,     0,     0,     0,  1331,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1316,  1326,  1400,  1313,     0,
       0,     0,  1541,  1350,     0,  1314,  1327,     0,     0,  1322,
    1323,  1324,  1325,  1318,  1326,     0,     0,  1315,     0,     0,
       0,     0,     0,  1124,  1327,  1337,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,     0,
    1541,     0,     0,     0,  1327,  1317,     0,     0,     0,     0,
       0,  1393,     0,  1346,     0,     0,     0,     0,     0,     0,
    1395,     0,     0,     0,     0,     0,     0,     0,     0,  1396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1404,  1405,  1406,  1407,  1408,     0,  1409,  1411,     0,     0,
    1412,  1413,     0,  1414,  1415,  1416,  1417,  1418,     0,  1419,
    1420,     0,     0,     0,  1423,  1424,     0,     0,     0,  1427,
    1428,     0,  1431,     0,     0,     0,  1432,     0,     0,     0,
    1433,  1434,     0,     0,     0,     0,  1435,  1436,  1437,  1438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1439,  1440,  1441,  1442,     0,  1443,  1444,  1445,  1446,
       0,     0,  1318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1341,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,     0,  1389,     0,  1541,
       0,     0,     0,  1327,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1447,  1448,     0,     0,     0,  1449,     0,
    1450,     0,     0,     0,  1451,  1452,  1453,  1454,     0,     0,
       0,  1455,  1456,     0,     0,     0,     0,     0,     0,     0,
       0,  1458,     0,  1459,  1460,  1461,   655,     0,     0,     0,
    1463,  1466,  1467,  1468,  1469,     0,  1470,     0,     0,     0,
       0,     0,  1471,  1472,     0,  1475,  1476,  1477,  1478,  1479,
    1480,  1481,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1482,     0,  1483,     0,  1551,     0,
       0,     0,     0,     0,     0,     0,  1484,  1485,  1486,     0,
    1487,     0,  1488,     0,  1489,     0,  1490,     0,     0,  1491,
    1492,  1493,  1494,  1536,  1498,  1499,     0,  1500,  1501,  1502,
       0,     0,     0,     0,     0,  1503,     0,  1505,  1506,  1508,
    1509,     0,     0,  1510,     0,  1511,  1512,  1513,  1514,  1515,
    1516,  1517,  1518,  1519,  1520,     0,   929,  1521,  1522,     0,
    1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,  1531,  1532,
       0,     0,     0,     0,     0,     0,     0,  1533,  1534,     0,
       0,  1462,     0,     0,     0,     0,  1465,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1474,
       0,   728,     0,     0,     0,  1291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1596,     0,  1411,  1548,  1597,
       0,     0,     0,  1614,     0,  1552,  1553,  1554,  1555,  1556,
    1557,  1558,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,     0,  1496,     0,
    1576,     0,  1548,     0,     0,     0,  1580,     0,     0,     0,
    1582,     0,  1583,  1584,  1585,  1464,     0,  1588,     0,     0,
    1590,     0,  1591,  1592,   155,     0,     0,     0,  1473,   648,
       0,     0,     0,  1598,     0,  1599,     0,  1600,     0,     0,
       0,  1603,     0,  1604,     0,     0,     0,   679,  1548,     0,
       0,  1612,     0,     0,  1563,  1615,     0,  1616,     0,     0,
    1619,     0,     0,  1620,   693,   694,   695,   697,   701,   702,
    1563,     0,     0,   157,     0,     0,     0,  1495,  1497,     0,
       0,     0,     0,     0,     0,   746,   751,   755,   759,     0,
     155,  1504,   697,  1507,     0,  1411,   681,   777,     0,   779,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   679,     0,     0,
       0,     0,     0,     0,     0,     0,  1661,     0,     0,     0,
    1306,     0,  1307,     0,   747,   752,   756,   760,     0,   157,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
       0,     0,   880,     0,     0,  1535,     0,   679,     0,     0,
     697,     0,     0,     0,     0,     0,   815,     0,     0,     0,
       0,     0,     0,   907,     0,     0,     0,   155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   155,     0,     0,   155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   889,  1615,     0,  1568,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   909,     0,     0,     0,   157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,   157,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1756,  1757,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1770,  1771,     0,     0,
       0,  1773,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1775,     0,     0,  1776,  1777,  1778,     0,
    1779,     0,  1780,     0,  1781,     0,  1782,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1783,  1784,
     948,  1786,     0,     0,     0,  1787,     0,     0,     0,     0,
       0,  1788,  1789,     0,     0,  1790,     0,     0,     0,     0,
       0,     0,     0,     0,  1791,  1792,     0,  1311,  1793,     0,
    1794,     0,     0,     0,     0,     0,  1795,  1796,  1797,  1798,
       0,     0,  1306,     0,  1307,     0,     0,     0,     0,   949,
       0,     0,     0,  1308,  1309,  1310,  1799,     0,     0,  1312,
       0,     0,     0,   156,     0,     0,     0,  1800,  1801,     0,
       0,     0,     0,     0,     0,  1802,  1313,     0,     0,     0,
       0,     0,     0,  1314,  1803,     0,   680,     0,     0,     0,
    1548,     0,  1808,     0,  1809,  1315,     0,  1810,  1811,  1812,
       0,     0,  1814,  1815,  1816,  1817,   698,  1818,  1819,     0,
    1316,  1820,     0,  1822,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1838,     0,     0,     0,     0,   156,
       0,   698,     0,     0,     0,     0,     0,     0,     0,  1306,
       0,  1307,     0,     0,     0,     0,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,     0,   814,     0,     0,     0,
    1317,     0,     0,  1873,     0,     0,  1874,     0,  1875,     0,
    1876,     0,     0,  1877,     0,     0,     0,     0,  1878,  1879,
       0,  1880,     0,  1881,     0,     0,  1882,  1883,     0,     0,
       0,  1884,  1885,  1886,     0,  1887,   888,     0,     0,   892,
    1889,     0,  1890,  1891,  1892,     0,  1893,  1894,  1895,  1896,
    1897,     0,   908,     0,     0,     0,   156,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   156,     0,     0,   156,     0,     0,     0,     0,     0,
       0,  1587,  1920,     0,  1921,     0,     0,     0,     0,  1922,
       0,     0,  1923,     0,     0,  1924,  1925,  1926,  1927,     0,
       0,     0,     0,     0,     0,     0,     0,  1936,  1236,  1311,
       0,     0,  1937,  1938,  1939,     0,     0,  1318,     0,     0,
    1943,  1944,     0,     0,  1946,     0,  1948,     0,  1950,  1343,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,  1312,     0,     0,  1541,     0,     0,     0,  1327,     0,
       0,     0,     0,     0,     0,     0,     0,  1237,  1313,     0,
       0,     0,  1292,     0,     0,  1314,     0,     0,     0,     0,
       0,     0,  1306,     0,  1307,     0,  1292,  1315,     0,     0,
       0,  1332,     0,  1308,  1309,  1310,     0,  1292,     0,     0,
    1292,     0,  1316,     0,     0,     0,     0,     0,     0,     0,
    1292,     0,     0,     0,  1292,     0,  1311,     0,     0,  1292,
       0,  1294,     0,     0,     0,     0,     0,  1292,     0,     0,
       0,     0,  1292,     0,     0,  1294,     0,     0,     0,     0,
    1359,     0,     0,     0,     0,     0,  1294,     0,  1312,  1294,
       0,     0,  1317,     0,  1292,     0,     0,     0,     0,  1294,
    1292,     0,     0,  1294,     0,  1313,     0,     0,  1294,     0,
       0,     0,  1314,  1292,     0,  1306,  1294,  1307,  1292,     0,
       0,  1294,  1292,     0,  1315,     0,  1308,  1309,  1310,  1292,
       0,     0,     0,     0,     0,  1306,     0,  1307,     0,  1316,
       0,     0,     0,  1294,     0,     0,  1308,  1309,  1310,  1294,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1294,  1306,     0,  1307,     0,  1294,  1292,     0,
       0,  1294,     0,     0,  1308,  1309,  1310,     0,  1294,     0,
       0,     0,     0,     0,  1292,     0,     0,     0,  1306,  1317,
       0,     0,     0,  1292,     0,     0,     0,     0,     0,  1308,
    1309,  1310,     0,     0,     0,     0,     0,     0,     0,  1318,
       0,     0,     0,     0,     0,     0,     0,  1294,  1292,     0,
       0,  1341,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,  1294,     0,     0,     0,     0,     0,     0,
    1327,     0,  1294,     0,     0,     0,     0,     0,     0,  1311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1292,  1294,     0,     0,
       0,     0,     0,     0,  1306,     0,  1307,     0,     0,     0,
       0,  1312,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,  1292,     0,     0,     0,  1313,     0,
     155,     0,     0,     0,     0,  1314,  1318,     0,     0,     0,
       0,     0,     0,     0,     0,  1294,     0,  1315,  1351,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,   155,  1326,     0,     0,
       0,  1425,  1316,  1541,     0,     0,   679,  1327,     0,     0,
       0,     0,  1311,  1294,     0,     0,     0,     0,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1312,   157,     0,     0,     0,     0,
     909,  1293,  1317,     0,     0,  1430,     0,     0,     0,     0,
    1311,  1313,     0,     0,  1312,  1293,     0,     0,  1314,     0,
       0,     0,     0,     0,     0,     0,  1293,     0,     0,  1293,
    1315,  1313,     0,     0,     0,  1311,     0,     0,  1314,  1293,
       0,     0,  1312,  1293,     0,  1316,     0,     0,  1293,     0,
    1315,     0,     0,     0,     0,     0,  1293,     0,     0,  1313,
       0,  1293,     0,     0,     0,  1316,  1314,  1312,     0,   908,
       0,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
       0,     0,     0,  1293,  1313,     0,     0,     0,     0,  1293,
       0,  1314,     0,  1316,     0,  1317,     0,     0,     0,     0,
       0,     0,  1293,  1315,     0,     0,     0,  1293,     0,     0,
       0,  1293,     0,     0,     0,  1317,     0,     0,  1293,  1318,
       0,  1311,     0,     0,     0,     0,     0,  1292,     0,     0,
       0,  1353,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,  1317,     0,     0,  1541,  1306,     0,  1307,
    1327,     0,     0,  1312,     0,     0,     0,  1293,  1308,  1309,
    1310,     0,     0,     0,     0,     0,     0,  1306,     0,  1307,
    1313,     0,     0,  1293,     0,     0,  1294,  1314,  1308,  1309,
    1310,     0,  1293,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1316,     0,     0,  1293,     0,     0,
       0,     0,  1318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1364,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1318,  1326,     0,     0,     0,     0,     0,  1541,
       0,     0,     0,  1327,  1366,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,  1317,  1293,  1306,     0,  1307,  1541,
    1318,     0,     0,  1327,     0,     0,     0,  1308,  1309,  1310,
       0,     0,  1368,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,  1306,  1293,  1307,     0,     0,  1541,     0,   156,
       0,  1327,     0,  1308,  1309,  1310,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,
       0,     0,     0,     0,     0,   156,  1327,     0,     0,     0,
       0,     0,     0,     0,     0,  1429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   697,
       0,   697,     0,     0,     0,   697,     0,     0,     0,     0,
       0,  1318,     0,     0,  1311,     0,     0,     0,     0,     0,
       0,     0,  1607,  1372,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,  1311,     0,     0,     0,  1541,     0,
       0,     0,  1327,     0,     0,     0,  1312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1313,     0,     0,  1312,     0,     0,     0,
    1314,  1608,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1315,  1313,     0,     0,     0,     0,     0,     0,
    1314,     0,     0,     0,     0,     0,     0,  1316,     0,     0,
       0,     0,  1315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1316,     0,     0,
       0,     0,     0,  1311,     0,   660,   661,   663,     0,     0,
       0,   664,   667,     0,     0,     0,  1293,     0,     0,     0,
       0,   686,     0,     0,     0,     0,     0,  1317,     0,  1311,
       0,   687,     0,     0,     0,  1312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1317,     0,     0,
       0,     0,  1313,     0,     0,     0,     0,     0,     0,  1314,
       0,  1312,   769,     0,     0,     0,     0,   778,     0,     0,
       0,  1315,     0,     0,     0,     0,     0,     0,  1313,   802,
     804,   805,   806,   807,     0,  1314,  1316,   816,     0,     0,
     818,   820,     0,     0,     0,     0,   829,  1315,     0,   835,
       0,   840,     0,   845,   847,   850,     0,   854,     0,     0,
     859,     0,  1316,     0,     0,     0,     0,     0,  1761,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   894,   895,     0,   898,   899,  1317,     0,     0,     0,
       0,     0,     0,   912,  1318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1374,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1318,  1326,     0,     0,     0,     0,
       0,  1541,     0,     0,     0,  1327,  1378,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,
       0,  1541,     0,     0,     0,  1327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     698,     0,     0,  1318,   698,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1382,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,  1806,  1326,     0,     0,     0,     0,  1318,
    1541,     0,     0,     0,  1327,     0,     0,     0,     0,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,     0,   955,   956,   958,   959,   960,
    1327,     0,   963,   964,   965,   966,     0,     0,     0,     0,
     971,   972,   973,   974,   975,   976,   977,   978,   979,   980,
     981,     0,     0,     0,   986,     0,     0,   697,   991,   992,
     993,   994,   995,   996,   997,   998,   999,  1000,  1001,  1002,
    1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,
    1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,
    1033,  1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,
    1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,
    1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,
    1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,
    1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,
    1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,
    1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,
    1103,     0,     0,     0,     0,     0,  1110,  1112,  1113,  1114,
       0,     0,     0,     0,  1119,  1120,  1121,     0,     0,  1127,
    1128,  1129,  1130,  1131,  1133,     0,     0,     0,     0,  1138,
    1139,  1140,     0,     0,     0,     0,     0,  1146,  1148,  1149,
    1150,  1151,  1152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1163,  1164,  1165,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1176,  1178,     0,  1180,  1181,
    1182,  1183,  1184,  1185,  1186,     0,     0,     0,  1190,     0,
    1192,  1194,  1196,  1198,  1199,     0,     0,     0,     0,     0,
       0,     0,  1207,     0,     0,  1211,  1212,  1213,  1214,  1215,
    1217,     0,     0,     0,  1222,  1223,  1225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1240,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1251,  1252,  1253,  1254,  1255,  1256,  1257,     0,     0,  1260,
    1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,
    1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,  1280,
    1281,   304,   305,   306,     0,     0,     0,     0,     0,     0,
       0,   307,   308,   309,   310,     0,   311,     0,     0,     0,
     312,   313,   314,   315,   316,     0,     0,     0,   317,     0,
       0,   318,     0,   319,     0,   320,     0,   321,   322,     0,
       0,   323,     0,     0,     0,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,     0,
       0,     0,   338,   339,   340,     0,     0,   341,   342,   343,
     344,     0,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,     0,     0,
       0,   459,   460,   461,     0,     0,   462,     0,     0,     0,
     463,   464,   465,   466,   467,   468,   469,   470,     0,   471,
     472,   473,   474,   475,     0,     0,     0,   476,     0,     0,
     477,   478,   479,     0,   480,     0,     0,     0,     0,   481,
       0,     0,     0,   482,   483,   484,   485,     0,     0,     0,
       0,     0,   486,   487,   488,   489,     0,     0,   490,     0,
       0,     0,     0,   491,   492,     0,   493,   494,   495,     0,
     496,   497,   498,   499,   500,   501,     0,     0,   502,   503,
     504,   505,     0,   506,     0,   507,   508,   509,   510,     0,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
       0,     0,   521,     0,     0,   522,     0,   523,     0,     0,
     524,     0,     0,     0,     0,     0,     0,   525,   526,   527,
     528,   529,   530,     0,     0,     0,   531,     0,   532,     0,
       0,   533,     0,     0,     0,     0,     0,     0,   534,   535,
       0,   536,   537,   538,   539,   540,     0,   541,     0,     0,
       0,     0,     0,   542,   543,     0,     0,   544,   545,   546,
     547,   548,     0,   549,     0,     0,     0,     0,   550,     0,
       0,     0,   551,   552,   553,   554,   555,     0,     0,     0,
       0,   556,   557,   558,   559,     0,     0,     0,   560,     0,
     561,   562,     0,   563,     0,     0,     0,     0,     0,   564,
       0,   565,     0,     0,     0,   566,     0,     0,   567,   568,
       0,     0,   569,   570,   571,   572,   573,     0,     0,   574,
       0,     0,     0,   575,     0,   576,   577,     0,     0,     0,
     578,   579,   580,   581,   582,   583,   584,   585,     0,   586,
     587,   588,   589,   590,   591,   592,   593,     0,     0,   594,
       0,   595,     0,   596,     0,   597,   598,   599,   600,   601,
     602,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,     0,     0,   613,     0,   614,     0,     0,     0,   615,
       0,     0,     0,     0,   616,   617,   618,   619,   620,   621,
     622,   623,   624,   147,     0,  1542,     0,     0,     0,   625,
     626,     0,     0,     0,     0,     0,  1410,   627,     0,   628,
     304,   305,   306,  1543,     0,     0,     0,     0,     0,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,  1544,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   628,  1545,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,  1544,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   628,  1577,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,  1544,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   628,  1609,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   628,  1338,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   670,  1338,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   683,  1338,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,  1344,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,  1345,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   670,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,  1410,   627,     0,   628,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,  1745,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   628,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,  1544,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   628,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   628,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   642,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   653,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   656,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   666,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   670,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   675,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   683,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   688,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   710,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   716,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   729,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   734,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   738,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   745,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   784,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   800,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   809,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   834,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   882,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,   910,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,   627,     0,  1123,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     0,     0,     0,     0,     0,   625,   626,
     304,   305,   306,     0,     0,     0,  1286,     0,   628,     0,
     307,   308,   309,   310,     0,   311,     0,     0,     0,   312,
     313,   314,   315,   316,     0,     0,     0,   317,     0,     0,
     318,     0,   319,     0,   320,     0,   321,   322,     0,     0,
     323,     0,     0,     0,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,     0,     0,
       0,   338,   339,   340,     0,     0,   341,   342,   343,   344,
       0,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,     0,     0,     0,
     459,   460,   461,     0,     0,   462,     0,     0,     0,   463,
     464,   465,   466,   467,   468,   469,   470,     0,   471,   472,
     473,   474,   475,     0,     0,     0,   476,     0,     0,   477,
     478,   479,     0,   480,     0,     0,     0,     0,   481,     0,
       0,     0,   482,   483,   484,   485,     0,     0,     0,     0,
       0,   486,   487,   488,   489,     0,     0,   490,     0,     0,
       0,     0,   491,   492,     0,   493,   494,   495,     0,   496,
     497,   498,   499,   500,   501,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,   508,   509,   510,     0,   511,
     512,   513,   514,   515,   516,   517,   518,   519,   520,     0,
       0,   521,     0,     0,   522,     0,   523,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,     0,     0,     0,   531,     0,   532,     0,     0,
     533,     0,     0,     0,     0,     0,     0,   534,   535,     0,
     536,   537,   538,   539,   540,     0,   541,     0,     0,     0,
       0,     0,   542,   543,     0,     0,   544,   545,   546,   547,
     548,     0,   549,     0,     0,     0,     0,   550,     0,     0,
       0,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     556,   557,   558,   559,     0,     0,     0,   560,     0,   561,
     562,     0,   563,     0,     0,     0,     0,     0,   564,     0,
     565,     0,     0,     0,   566,     0,     0,   567,   568,     0,
       0,   569,   570,   571,   572,   573,     0,     0,   574,     0,
       0,     0,   575,     0,   576,   577,     0,     0,     0,   578,
     579,   580,   581,   582,   583,   584,   585,     0,   586,   587,
     588,   589,   590,   591,   592,   593,     0,     0,   594,     0,
     595,     0,   596,     0,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
       0,     0,   613,     0,   614,     0,     0,     0,   615,     0,
       0,     0,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   147,     1,     0,     0,     0,     2,     3,     0,
       0,     0,     0,     4,     0,     0,   627,     0,   628,     0,
       0,     0,     0,     5,     6,     7,     0,     8,     9,     0,
      10,     0,    11,     0,    12,     0,     0,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,    19,    20,
       0,     0,     0,    21,    22,     0,     0,     0,     0,    23,
       0,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1306,     0,  1307,     0,     0,     0,     0,     0,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
       0,     0,  1306,     0,  1307,     0,     0,     0,     0,     0,
       0,     0,     0,  1308,  1309,  1310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    36,    37,     0,
       0,     0,    38,     0,     0,    39,    40,    41,     0,     0,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,     0,    43,    44,    45,     0,    46,    47,     0,     0,
       0,     0,     0,    48,     0,    49,    50,     0,    51,    52,
      53,     0,     0,     0,     0,    54,    55,    56,    57,    58,
       0,     0,     0,     0,    59,    60,     0,    61,    62,    63,
      64,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,    66,    67,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,     0,    71,     0,    72,    73,     0,    74,
      75,    76,    77,    78,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,     0,    82,     0,    83,    84,     0,
      85,    86,    87,    88,    89,    90,     0,     0,    91,     0,
       0,     0,     0,     0,    92,     0,    93,     0,    94,    95,
      96,     0,     0,    97,    98,     0,     0,  1311,     0,     0,
      99,     0,   100,   101,     0,     0,     0,   102,   103,   104,
       0,     0,     0,     0,     0,   105,   106,   107,   108,  1311,
       0,     0,     0,   109,   110,   111,     0,   112,     0,  1312,
     113,     0,   114,   115,   116,   117,   118,     0,   119,     0,
     120,   121,   122,     0,   123,   124,  1313,     0,   125,   126,
       0,  1312,     0,  1314,     0,   127,     0,     0,     0,   128,
     129,     0,   130,     0,     0,  1315,   131,     0,  1313,     0,
       0,     0,     0,     0,     0,  1314,   132,     0,     0,     0,
    1316,     0,     0,     0,     0,   133,   134,  1315,   135,     0,
     136,     0,   137,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1316,     0,     0,     0,     0,     0,     0,   138,
     139,     0,   140,     0,   141,   142,   143,     0,   144,   145,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1317,   147,   148,     1,     0,     0,     0,     2,     3,     0,
       0,     0,     0,     4,     0,     0,     0,   149,     0,     0,
       0,   150,  1317,     5,     6,     7,     0,     8,     9,     0,
      10,     0,    11,     0,    12,     0,     0,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,    19,    20,
       0,     0,     0,    21,    22,     0,     0,     0,     0,    23,
       0,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1386,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1318,
       0,     0,     0,     0,  1541,     0,     0,     0,  1327,     0,
       0,  1388,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,     0,     0,  1541,     0,     0,     0,
    1327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    36,    37,     0,
       0,     0,    38,     0,     0,    39,    40,    41,     0,     0,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,     0,    43,    44,    45,     0,    46,    47,     0,     0,
       0,     0,     0,    48,     0,    49,    50,     0,    51,    52,
      53,     0,     0,     0,     0,    54,    55,    56,    57,    58,
       0,     0,     0,     0,    59,    60,     0,    61,    62,    63,
      64,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,    66,    67,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,     0,    71,     0,    72,    73,     0,    74,
      75,    76,    77,    78,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,     0,    82,     0,    83,    84,     0,
      85,    86,    87,    88,    89,    90,     0,     0,    91,     0,
       0,     0,     0,     0,    92,     0,    93,     0,    94,    95,
     917,     0,     0,    97,    98,     0,     0,     0,     0,     0,
      99,     0,   100,   101,     0,     0,     0,   102,   103,   104,
       0,     0,     0,     0,     0,   105,   106,   107,   108,     0,
       0,     0,     0,   109,   110,   111,     0,   112,     0,     0,
     113,     0,   114,   115,   116,   117,   118,     0,   119,     0,
     120,   121,   122,     0,   123,   124,     0,     0,   125,   126,
       0,     0,     0,     0,     0,   127,     0,     0,     0,   128,
     129,     0,   130,     0,     0,     0,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   132,     0,     0,     0,
       0,     0,     0,     0,     0,   133,   134,     0,   135,     0,
     136,     0,   137,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,     2,     3,     0,     0,   138,
     139,     4,   140,     0,   141,   142,   143,     0,   144,   145,
     146,     5,     6,     7,     0,     8,     9,     0,    10,     0,
      11,   147,    12,     0,     0,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,   149,     0,     0,
       0,   150,     0,     0,     0,    18,    19,    20,     0,     0,
       0,    21,    22,     0,     0,     0,     0,    23,     0,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1306,     0,
    1307,     0,     0,     0,     0,     0,     0,     0,     0,  1308,
    1309,  1310,     0,     0,     0,     0,     0,     0,     0,     0,
    1306,     0,  1307,     0,     0,     0,     0,     0,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    35,    36,    37,     0,     0,     0,
      38,     0,     0,    39,    40,    41,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
      43,    44,    45,     0,    46,    47,     0,     0,  1306,     0,
    1307,    48,     0,    49,    50,     0,    51,    52,    53,  1308,
    1309,  1310,     0,    54,    55,    56,    57,    58,  1306,     0,
    1307,     0,    59,    60,     0,    61,    62,    63,    64,  1308,
    1309,  1310,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,    66,    67,  1306,     0,  1307,     0,     0,     0,
      68,     0,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,     0,    71,     0,    72,    73,     0,    74,    75,    76,
      77,    78,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,     0,    82,     0,    83,    84,  1618,    85,    86,
      87,    88,    89,    90,     0,     0,    91,     0,     0,     0,
       0,     0,    92,     0,    93,     0,    94,    95,     0,     0,
       0,    97,    98,     0,     0,  1311,     0,     0,    99,     0,
     100,   101,     0,     0,     0,   102,   103,   104,     0,     0,
       0,     0,     0,   105,   106,   107,   108,  1311,     0,     0,
       0,   109,   110,   111,     0,   112,     0,  1312,   113,     0,
     114,   115,   116,   117,   118,     0,   119,     0,   120,   121,
     122,     0,   123,   124,  1313,     0,   125,   126,     0,  1312,
       0,  1314,     0,   127,     0,     0,     0,   128,   129,     0,
     130,     0,     0,  1315,   131,     0,  1313,     0,     0,     0,
       0,     0,     0,  1314,   132,     0,     0,     0,  1316,     0,
       0,     0,     0,   133,   134,  1315,   135,     0,   136,     0,
     137,     0,     0,     0,     0,  1311,     0,     0,     0,     0,
    1316,     0,     0,     0,     0,     0,     0,   138,   139,     0,
     140,     0,   141,   142,   143,  1311,   144,   145,   146,     0,
       0,     0,     0,     0,     0,     0,     0,  1312,  1317,   147,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1311,     0,     0,  1313,   149,     0,  1312,     0,   150,
    1317,  1314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1315,  1313,     0,     0,     0,  1306,     0,
    1307,  1314,     0,  1312,     0,     0,     0,     0,  1316,  1308,
    1309,  1310,     0,  1315,     0,     0,     0,     0,     0,     0,
    1313,     0,     0,  1306,     0,  1307,     0,  1314,  1316,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,  1315,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1306,     0,  1307,  1316,     0,     0,     0,  1317,     0,
       0,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,  1306,     0,  1307,     0,  1318,     0,     0,  1317,     0,
       0,     0,  1308,  1309,  1310,     0,     0,  1341,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,  1318,     0,     0,
       0,     0,     0,     0,  1317,     0,  1327,     0,     0,  1394,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,  1306,  1541,  1307,     0,     0,  1327,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,  1306,     0,  1307,     0,     0,     0,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1306,     0,  1307,     0,
       0,     0,     0,     0,     0,  1318,     0,  1308,  1309,  1310,
       0,     0,     0,     0,     0,     0,     0,  1622,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1318,  1326,     0,  1623,     0,
       0,     0,     0,     0,     0,     0,  1327,  1634,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,
       0,  1318,  1635,     0,     0,  1311,  1327,     0,     0,     0,
       0,     0,     0,  1639,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,     0,  1640,     0,
    1311,     0,  1327,     0,     0,     0,     0,  1312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1313,     0,     0,     0,  1311,     0,
       0,  1314,  1312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1315,     0,     0,     0,     0,  1311,  1313,
       0,     0,     0,     0,     0,     0,  1314,     0,  1316,     0,
    1312,     0,  1306,     0,  1307,     0,     0,     0,  1315,     0,
       0,     0,     0,  1308,  1309,  1310,     0,  1313,     0,     0,
    1312,     0,     0,  1316,  1314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1315,  1313,     0,     0,
    1311,     0,     0,     0,  1314,     0,     0,     0,  1317,     0,
       0,  1316,     0,     0,     0,     0,  1315,     0,     0,     0,
    1311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1316,  1312,  1317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1311,     0,     0,     0,     0,     0,  1313,
       0,     0,  1312,     0,     0,     0,  1314,     0,     0,     0,
       0,  1317,     0,     0,     0,     0,     0,     0,  1315,  1313,
       0,     0,     0,     0,     0,  1312,  1314,     0,     0,     0,
       0,  1317,     0,  1316,     0,     0,     0,     0,  1315,  1306,
       0,  1307,  1313,     0,     0,     0,     0,     0,     0,  1314,
    1308,  1309,  1310,  1316,     0,     0,     0,     0,     0,     0,
       0,  1315,     0,     0,     0,     0,     0,  1306,     0,  1307,
       0,     0,     0,     0,     0,  1318,  1316,     0,  1308,  1309,
    1310,     0,     0,  1317,     0,     0,     0,  1641,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,
    1318,     0,  1642,  1317,     0,     0,  1327,     0,     0,     0,
       0,     0,  1643,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,     0,     0,     0,  1317,  1644,  1318,     0,
       0,  1327,     0,     0,     0,     0,     0,     0,     0,  1311,
    1645,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1318,  1326,
       0,     0,     0,     0,     0,  1646,     0,     0,     0,  1327,
    1647,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,  1312,     0,     0,     0,  1648,     0,     0,     0,  1327,
       0,     0,     0,     0,     0,  1306,     0,  1307,  1313,     0,
       0,     0,     0,     0,     0,  1314,  1308,  1309,  1310,     0,
    1318,     0,     0,     0,     0,     0,     0,  1315,     0,     0,
       0,     0,  1457,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1318,  1326,  1316,     0,     0,     0,     0,  1541,     0,     0,
       0,  1327,  1662,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,  1318,     0,     0,     0,  1663,     0,     0,
       0,  1327,     0,     0,     0,  1343,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,  1311,     0,     0,     0,
    1671,     0,  1317,     0,  1327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1306,     0,  1307,     0,  1311,     0,     0,     0,  1312,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1313,     0,     0,     0,     0,
       0,     0,  1314,     0,     0,  1306,  1312,  1307,     0,     0,
       0,     0,     0,     0,  1315,     0,  1308,  1309,  1310,     0,
       0,     0,     0,  1313,     0,  1306,     0,  1307,     0,  1316,
    1314,     0,     0,     0,     0,     0,  1308,  1309,  1310,     0,
       0,     0,  1315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1317,
       0,  1691,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,  1306,  1311,  1307,     0,     0,  1692,     0,     0,     0,
    1327,     0,  1308,  1309,  1310,     0,     0,  1317,     0,     0,
       0,     0,     0,     0,     0,     0,  1306,     0,  1307,     0,
       0,     0,     0,     0,  1312,     0,     0,  1308,  1309,  1310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1313,     0,     0,  1306,     0,  1307,     0,  1314,     0,
       0,     0,     0,     0,     0,  1308,  1309,  1310,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1318,  1311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1341,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,
       0,     0,     0,  1704,  1318,  1317,     0,  1327,     0,  1312,
       0,     0,  1311,     0,     0,     0,  1341,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,  1313,     0,     0,     0,
       0,  1711,  1311,  1314,     0,  1327,     0,     0,     0,     0,
       0,     0,     0,     0,  1312,  1315,  1306,     0,  1307,     0,
       0,     0,     0,     0,     0,     0,     0,  1308,  1309,  1310,
    1316,  1313,     0,     0,  1312,     0,     0,     0,  1314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1315,  1313,     0,     0,     0,     0,     0,     0,  1314,     0,
       0,     0,     0,     0,     0,  1316,     0,     0,  1311,     0,
    1315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1317,     0,     0,     0,     0,  1316,     0,     0,     0,     0,
       0,     0,  1318,  1311,     0,     0,     0,     0,     0,     0,
    1312,     0,     0,     0,  1341,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,  1317,     0,  1313,     0,  1714,
    1306,  1311,  1307,  1327,  1314,  1312,     0,     0,     0,     0,
       0,  1308,  1309,  1310,     0,  1317,  1315,     0,     0,     0,
       0,     0,  1313,     0,     0,     0,     0,     0,     0,  1314,
       0,  1316,     0,  1312,     0,  1306,     0,  1307,     0,     0,
       0,  1315,     0,     0,     0,     0,  1308,  1309,  1310,     0,
    1313,     0,     0,     0,     0,     0,  1316,  1314,     0,     0,
    1306,     0,  1307,     0,     0,     0,     0,     0,     0,  1315,
       0,  1308,  1309,  1310,     0,     0,     0,  1318,     0,     0,
       0,  1317,     0,     0,  1316,     0,     0,     0,     0,  1729,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,     0,  1730,     0,  1317,     0,  1327,     0,
       0,     0,  1318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1311,  1824,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1318,  1326,  1317,     0,     0,     0,     0,  1825,
       0,     0,     0,  1327,  1827,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,  1306,  1312,  1307,     0,     0,  1828,
       0,     0,     0,  1327,     0,  1308,  1309,  1310,     0,     0,
       0,     0,  1313,     0,  1306,     0,  1307,     0,     0,  1314,
       0,     0,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,  1315,     0,     0,     0,     0,     0,     0,  1318,     0,
       0,     0,     0,     0,     0,     0,  1316,     0,     0,     0,
    1829,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,  1318,     0,  1830,     0,  1311,     0,  1327,
       0,     0,     0,     0,     0,  1831,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,     0,
    1832,  1318,     0,     0,  1327,     0,  1317,     0,     0,  1312,
       0,     0,  1311,  1834,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,  1313,     0,  1835,     0,
       0,     0,  1327,  1314,     0,     0,     0,  1311,     0,     0,
       0,     0,     0,     0,  1312,  1315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1316,  1313,     0,  1306,     0,  1307,     0,     0,  1314,  1312,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
    1315,     0,     0,     0,     0,     0,  1313,     0,     0,     0,
       0,     0,     0,  1314,     0,  1316,  1306,     0,  1307,     0,
       0,     0,     0,     0,     0,  1315,     0,  1308,  1309,  1310,
    1317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1316,     0,     0,  1318,     0,     0,     0,     0,     0,     0,
       0,  1311,     0,     0,     0,  1840,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,  1317,     0,     0,     0,     0,
    1841,  1311,     0,     0,  1327,     0,     0,     0,     0,     0,
       0,     0,  1306,  1312,  1307,     0,     0,     0,     0,     0,
    1317,     0,     0,  1308,  1309,  1310,     0,     0,     0,     0,
    1313,     0,  1306,  1312,  1307,     0,     0,  1314,     0,     0,
       0,     0,     0,  1308,  1309,  1310,     0,     0,     0,  1315,
    1313,     0,     0,     0,  1306,     0,  1307,  1314,     0,     0,
       0,     0,     0,     0,  1316,  1308,  1309,  1310,     0,  1315,
       0,     0,     0,     0,  1306,     0,  1307,  1318,     0,     0,
       0,     0,     0,     0,  1316,  1308,  1309,  1310,     0,  1842,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,     0,  1843,     0,     0,     0,  1327,     0,
       0,     0,  1318,     0,  1317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1844,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,  1317,     0,     0,  1318,     0,  1845,
    1311,     0,     0,  1327,     0,     0,     0,     0,     0,  1848,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,     0,  1849,     0,     0,     0,  1327,  1306,
       0,  1307,  1312,  1311,     0,     0,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,     0,     0,     0,     0,  1313,
       0,     0,     0,     0,     0,     0,  1314,     0,     0,     0,
       0,     0,     0,     0,     0,  1312,     0,     0,  1315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1313,  1316,     0,     0,     0,     0,     0,  1314,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,  1311,
       0,  1315,     0,  1856,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1318,  1326,     0,     0,     0,  1316,     0,  1857,  1311,
       0,     0,  1327,  1905,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1312,  1326,  1317,     0,     0,     0,     0,  1906,     0,
       0,  1311,  1327,     0,     0,     0,     0,     0,  1313,     0,
       0,  1312,     0,     0,     0,  1314,     0,     0,     0,     0,
       0,  1311,     0,     0,     0,     0,  1317,  1315,  1313,     0,
       0,     0,  1306,  1312,  1307,  1314,     0,     0,     0,     0,
       0,     0,  1316,  1308,  1309,  1310,     0,  1315,     0,     0,
    1313,     0,  1306,  1312,  1307,     0,     0,  1314,     0,     0,
       0,     0,  1316,  1308,  1309,  1310,     0,     0,     0,  1315,
    1313,     0,     0,     0,     0,     0,     0,  1314,     0,     0,
       0,     0,     0,     0,  1316,     0,     0,     0,     0,  1315,
       0,     0,  1317,     0,  1306,     0,  1307,     0,     0,     0,
       0,     0,     0,     0,  1316,  1308,  1309,  1310,     0,     0,
    1318,     0,  1317,     0,     0,     0,  1311,     0,     0,     0,
       0,     0,  1910,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,     0,  1317,     0,     0,  1911,     0,     0,
    1306,  1327,  1307,  1318,     0,     0,     0,     0,  1312,     0,
       0,  1308,  1309,  1310,  1317,  1913,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,  1313,     0,     0,     0,     0,
    1914,     0,  1314,     0,  1327,  1306,     0,  1307,     0,     0,
       0,     0,     0,     0,  1315,     0,  1308,  1309,  1310,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1316,
       0,  1306,     0,  1307,     0,     0,     0,     0,     0,  1318,
       0,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,  1928,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1318,
    1326,     0,     0,     0,     0,     0,  1929,     0,     0,     0,
    1327,  1331,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1317,
    1326,  1318,     0,     0,     0,     0,     0,     0,     0,  1311,
    1327,     0,     0,  1337,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1318,  1326,     0,     0,     0,     0,     0,     0,  1311,
       0,     0,  1327,  1341,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1312,  1326,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1327,     0,     0,     0,     0,     0,  1313,     0,
       0,  1312,     0,     0,     0,  1314,     0,     0,     0,     0,
       0,  1311,     0,     0,     0,     0,     0,  1315,  1313,     0,
       0,     0,     0,     0,     0,  1314,     0,     0,     0,     0,
       0,     0,  1316,     0,     0,     0,     0,  1315,     0,     0,
       0,     0,     0,  1312,     0,     0,     0,     0,     0,     0,
       0,     0,  1316,     0,     0,     0,  1318,  1311,     0,     0,
    1313,     0,     0,     0,     0,     0,     0,  1314,  1343,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,  1315,
       0,     0,  1317,     0,     0,     0,     0,  1327,     0,  1312,
       0,     0,  1311,     0,  1316,     0,     0,     0,     0,     0,
       0,     0,  1317,     0,     0,     0,  1313,     0,     0,     0,
       0,     0,     0,  1314,     0,     0,     0,  1306,  1311,  1307,
       0,     0,     0,     0,  1312,  1315,     0,     0,  1308,  1309,
    1310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1316,  1313,     0,  1306,  1317,  1307,     0,     0,  1314,     0,
    1312,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,  1313,     0,  1306,
       0,  1307,     0,     0,  1314,  1316,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,     0,  1315,     0,     0,     0,
    1317,  1306,     0,  1307,     0,     0,     0,     0,     0,  1318,
       0,  1316,  1308,  1309,  1310,  1362,     0,     0,     0,     0,
       0,  1351,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1318,
    1326,     0,     0,     0,     0,  1317,     0,     0,     0,     0,
    1327,  1353,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,  1306,     0,  1307,     0,     0,     0,     0,
    1327,  1317,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,  1306,     0,  1307,     0,     0,     0,     0,
       0,     0,  1327,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1364,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1327,     0,
       0,     0,  1318,     0,  1311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1366,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,     0,     0,  1318,     0,
    1311,     0,     0,  1327,     0,     0,  1312,     0,     0,     0,
    1368,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,  1313,     0,  1306,  1311,  1307,     0,  1327,
    1314,     0,  1312,     0,     0,     0,  1308,  1309,  1310,     0,
       0,     0,  1315,     0,     0,     0,     0,     0,  1311,  1313,
    1306,     0,  1307,     0,     0,     0,  1314,  1316,  1312,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,  1315,     0,
       0,     0,     0,     0,     0,  1313,     0,     0,     0,     0,
    1312,     0,  1314,  1316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1315,     0,     0,  1313,     0,     0,
    1311,     0,     0,     0,  1314,     0,     0,  1317,     0,  1316,
       0,     0,     0,     0,     0,     0,  1315,  1306,     0,  1307,
       0,     0,     0,     0,     0,     0,     0,     0,  1308,  1309,
    1310,  1316,  1312,  1317,     0,     0,     0,     0,     0,     0,
    1311,     0,     0,     0,     0,     0,     0,     0,     0,  1313,
       0,     0,     0,     0,     0,     0,  1314,     0,     0,  1317,
       0,     0,     0,     0,  1306,     0,  1307,     0,  1315,     0,
       0,     0,  1312,     0,     0,  1308,  1309,  1310,     0,     0,
       0,  1317,     0,  1316,     0,     0,     0,     0,     0,  1313,
    1306,     0,  1307,     0,     0,     0,  1314,     0,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,  1315,     0,
       0,     0,     0,     0,     0,     0,  1306,     0,  1307,     0,
       0,     0,     0,  1316,  1318,     0,     0,  1308,  1309,  1310,
       0,     0,     0,  1385,     0,     0,  1372,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,
    1318,     0,  1311,     0,     0,  1327,     0,     0,     0,     0,
       0,     0,  1374,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,  1317,     0,     0,  1318,  1311,     0,     0,
       0,  1327,     0,     0,  1312,     0,     0,     0,  1378,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1318,     0,
       0,  1313,     0,     0,     0,     0,     0,  1327,  1314,  1312,
    1382,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
    1315,     0,     0,     0,     0,     0,  1313,     0,     0,  1327,
       0,     0,     0,  1314,     0,  1316,     0,     0,     0,     0,
    1306,     0,  1307,     0,  1311,  1315,     0,     0,     0,     0,
    1318,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
    1316,     0,  1386,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,     0,     0,     0,  1312,     0,     0,     0,
       0,  1327,     0,     0,     0,  1317,     0,     0,     0,     0,
    1318,  1311,     0,  1313,     0,     0,     0,     0,     0,     0,
    1314,     0,  1388,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1401,  1326,  1315,     0,     0,     0,     0,  1311,     0,     0,
    1306,  1327,  1307,  1312,     0,     0,     0,  1316,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
    1313,     0,  1306,  1311,  1307,     0,     0,  1314,     0,  1312,
       0,     0,     0,  1308,  1309,  1310,     0,     0,     0,  1315,
       0,     0,     0,     0,     0,     0,  1313,     0,  1306,     0,
    1307,     0,     0,  1314,  1316,  1312,     0,  1317,     0,  1308,
    1309,  1310,     0,     0,     0,  1315,     0,     0,     0,     0,
       0,     0,  1313,     0,  1306,     0,  1307,     0,     0,  1314,
    1316,     0,  1318,     0,     0,  1308,  1309,  1310,     0,     0,
       0,  1315,     0,     0,  1394,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,  1317,     0,  1316,  1318,     0,     0,
       0,     0,     0,  1327,     0,     0,     0,     0,     0,  1386,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
    1317,     0,     0,     0,     0,     0,     0,     0,  1327,     0,
       0,     0,     0,     0,     0,     0,  1306,  1311,  1307,     0,
       0,     0,     0,     0,     0,     0,  1317,  1308,  1309,  1310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1306,     0,  1307,  1318,     0,     0,     0,     0,  1312,
       0,     0,  1308,  1309,  1310,     0,  1457,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,  1313,     0,     0,     0,
       0,     0,     0,  1314,     0,  1327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1315,     0,     0,     0,     0,
       0,  1318,     0,     0,     0,     0,     0,  1311,     0,     0,
    1316,     0,     0,  1386,  1319,  1320,  1321,  1540,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,  1318,     0,  1311,
       0,     0,  1327,     0,     0,     0,     0,     0,     0,  1312,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,  1318,  1541,  1311,  1313,     0,  1327,     0,
    1317,  1312,     0,  1314,     0,     0,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,  1315,     0,     0,  1313,     0,
    1624,  1311,     0,     0,  1327,  1314,     0,  1312,     0,     0,
    1316,     0,     0,     0,     0,     0,     0,  1315,     0,     0,
       0,     0,     0,     0,  1313,     0,     0,     0,     0,     0,
       0,  1314,  1316,  1312,     0,  1306,     0,  1307,     0,     0,
       0,     0,     0,  1315,     0,     0,  1308,  1309,  1310,     0,
    1313,     0,     0,     0,     0,     0,     0,  1314,  1316,     0,
    1317,     0,     0,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,     0,  1311,     0,  1306,     0,  1307,     0,     0,
       0,     0,  1317,     0,  1316,     0,  1308,  1309,  1310,     0,
       0,     0,     0,     0,     0,     0,     0,  1318,  1311,     0,
       0,     0,     0,     0,     0,  1312,     0,     0,  1317,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,  1313,     0,  1625,     0,     0,     0,  1327,  1314,
    1312,     0,     0,  1306,  1317,  1307,     0,     0,     0,     0,
       0,  1315,     0,     0,  1308,  1309,  1310,  1313,     0,     0,
       0,     0,     0,     0,  1314,     0,  1316,     0,     0,     0,
       0,  1306,     0,  1307,     0,     0,  1315,     0,     0,     0,
       0,     0,  1308,  1309,  1310,     0,     0,  1318,     0,     0,
       0,  1316,     0,     0,     0,     0,     0,     0,     0,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1318,
       0,     0,     0,     0,  1626,     0,  1317,     0,  1327,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,  1306,     0,  1307,  1318,  1627,     0,     0,     0,
    1327,  1317,     0,  1308,  1309,  1310,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,
       0,  1318,  1630,     0,     0,     0,  1327,     0,     0,     0,
       0,     0,  1311,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,  1306,  1631,  1307,
       0,     0,  1327,     0,     0,     0,     0,     0,  1308,  1309,
    1310,     0,     0,     0,  1312,     0,     0,     0,     0,     0,
       0,     0,  1311,     0,     0,     0,     0,     0,     0,     0,
       0,  1313,     0,     0,     0,     0,     0,     0,  1314,     0,
    1306,     0,  1307,  1318,     0,     0,     0,     0,     0,     0,
    1315,  1308,  1309,  1310,  1312,     0,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,  1316,     0,     0,  1318,     0,
    1632,  1313,     0,     0,  1327,     0,     0,     0,  1314,     0,
    1311,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
    1315,     0,     0,     0,     0,  1633,     0,     0,     0,  1327,
       0,     0,     0,     0,     0,  1316,     0,  1306,  1311,  1307,
       0,     0,  1312,     0,     0,  1317,     0,     0,  1308,  1309,
    1310,     0,     0,     0,     0,     0,     0,     0,     0,  1313,
       0,     0,     0,     0,     0,     0,  1314,     0,     0,     0,
    1312,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
       0,     0,     0,     0,     0,  1317,     0,  1313,     0,     0,
       0,     0,     0,  1316,  1314,     0,  1306,     0,  1307,  1311,
       0,     0,     0,     0,     0,     0,  1315,  1308,  1309,  1310,
       0,     0,     0,     0,     0,     0,  1306,     0,  1307,     0,
       0,  1316,     0,     0,     0,     0,     0,  1308,  1309,  1310,
       0,  1312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1317,     0,     0,     0,     0,  1313,     0,
       0,     0,     0,     0,  1311,  1314,     0,  1306,     0,  1307,
       0,     0,  1318,     0,     0,     0,     0,  1315,  1308,  1309,
    1310,  1317,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1316,  1326,     0,     0,  1312,     0,     0,  1636,
       0,     0,     0,  1327,     0,     0,     0,  1311,     0,     0,
       0,     0,  1318,  1313,     0,     0,     0,     0,     0,     0,
    1314,     0,     0,     0,  1637,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1315,  1326,     0,     0,     0,     0,     0,  1312,
       0,     0,  1317,  1327,     0,     0,     0,  1316,     0,     0,
       0,     0,     0,  1306,     0,  1307,  1313,     0,     0,     0,
       0,     0,     0,  1314,  1308,  1309,  1310,     0,     0,     0,
    1318,     0,     0,     0,  1311,  1315,     0,     0,     0,     0,
       0,     0,  1638,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1316,  1326,     0,     0,     0,     0,     0,  1317,  1318,     0,
       0,  1327,     0,     0,     0,     0,  1312,     0,     0,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,  1313,     0,  1649,     0,     0,     0,  1327,
    1314,     0,  1306,  1311,  1307,     0,     0,     0,     0,     0,
    1317,     0,  1315,  1308,  1309,  1310,     0,     0,     0,     0,
       0,     0,     0,  1311,     0,     0,     0,  1316,     0,  1318,
       0,     0,     0,     0,     0,  1312,     0,     0,     0,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,  1313,     0,     0,  1312,  1650,     0,     0,  1314,
    1327,     0,     0,     0,  1311,     0,     0,     0,     0,     0,
       0,  1315,  1313,     0,     0,     0,     0,  1317,     0,  1314,
       0,  1306,     0,  1307,  1318,     0,  1316,     0,     0,     0,
       0,  1315,  1308,  1309,  1310,     0,  1312,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,  1316,     0,     0,     0,
    1306,  1651,  1307,  1313,     0,  1327,     0,     0,     0,     0,
    1314,  1308,  1309,  1310,     0,     0,     0,  1318,     0,     0,
       0,     0,  1315,     0,     0,     0,  1317,     0,     0,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,  1316,  1326,     0,
    1311,     0,     0,  1306,  1652,  1307,  1317,     0,  1327,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1306,     0,  1307,     0,
       0,     0,  1312,     0,     0,     0,     0,  1308,  1309,  1310,
       0,     0,     0,     0,  1318,     0,     0,  1317,     0,  1313,
       0,     0,     0,     0,     0,     0,  1314,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,     0,  1315,     0,
       0,  1653,     0,     0,     0,  1327,     0,     0,  1306,  1311,
    1307,     0,     0,  1316,     0,     0,     0,     0,     0,  1308,
    1309,  1310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1318,     0,     0,     0,     0,     0,     0,
       0,  1312,     0,     0,     0,     0,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,  1318,  1326,     0,     0,     0,  1313,     0,
    1654,     0,     0,  1317,  1327,  1314,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,     0,  1315,     0,     0,
    1655,     0,     0,     0,  1327,     0,     0,     0,  1311,     0,
       0,     0,  1316,     0,  1318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,  1311,     0,     0,
    1312,  1656,     0,     0,     0,  1327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1313,     0,     0,
       0,     0,  1317,     0,  1314,     0,     0,     0,     0,  1312,
       0,     0,     0,     0,     0,     0,  1315,     0,     0,     0,
    1311,     0,     0,     0,     0,     0,  1313,     0,     0,     0,
       0,  1316,     0,  1314,     0,     0,     0,     0,     0,     0,
    1318,     0,     0,  1311,     0,  1315,     0,     0,     0,     0,
       0,     0,  1312,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1316,  1326,     0,     0,     0,     0,     0,  1657,     0,  1313,
       0,  1327,  1306,     0,  1307,  1312,  1314,     0,     0,     0,
       0,  1317,     0,  1308,  1309,  1310,     0,     0,  1315,     0,
       0,     0,  1313,     0,  1306,  1311,  1307,     0,     0,  1314,
       0,     0,     0,  1316,     0,  1308,  1309,  1310,     0,     0,
    1317,  1315,     0,     0,     0,     0,     0,     0,     0,  1318,
       0,     0,     0,     0,     0,     0,  1316,  1312,     0,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,  1313,     0,  1658,  1306,     0,  1307,
    1327,  1314,     0,  1317,     0,     0,     0,     0,  1308,  1309,
    1310,     0,     0,  1315,     0,     0,     0,     0,     0,  1306,
       0,  1307,     0,     0,     0,     0,  1317,     0,  1316,     0,
    1308,  1309,  1310,     0,     0,     0,     0,     0,     0,     0,
       0,  1306,     0,  1307,     0,     0,     0,     0,  1318,     0,
       0,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
    1659,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,  1306,     0,  1307,     0,     0,     0,  1318,  1317,  1327,
       0,     0,  1308,  1309,  1310,     0,     0,     0,     0,  1660,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1306,
       0,  1307,     0,     0,     0,     0,     0,     0,  1327,     0,
    1308,  1309,  1310,     0,     0,     0,     0,     0,     0,     0,
    1318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,  1318,     0,     0,     0,  1664,     0,  1311,
       0,  1327,     0,     0,     0,     0,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,  1306,     0,  1307,     0,     0,
    1665,  1311,     0,     0,  1327,     0,  1308,  1309,  1310,     0,
       0,  1312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1318,     0,     0,  1313,     0,
       0,     0,     0,  1312,     0,  1314,     0,  1666,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,  1315,     0,     0,
    1313,     0,     0,     0,  1311,     0,  1327,  1314,     0,     0,
       0,     0,  1316,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,     0,     0,     0,     0,  1311,     0,     0,     0,
       0,     0,     0,     0,  1316,     0,  1312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1311,     0,
       0,     0,     0,  1313,     0,     0,     0,     0,  1312,     0,
    1314,     0,  1317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1315,     0,     0,  1313,     0,     0,  1311,     0,
    1312,     0,  1314,     0,  1317,     0,     0,  1316,     0,     0,
       0,     0,     0,     0,  1315,     0,     0,  1313,     0,     0,
       0,     0,     0,     0,  1314,     0,  1311,     0,     0,  1316,
    1312,     0,     0,     0,     0,     0,  1315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1313,     0,     0,
       0,  1316,     0,     0,  1314,     0,     0,  1317,  1312,     0,
       0,     0,     0,     0,     0,     0,  1315,     0,     0,     0,
       0,     0,     0,     0,     0,  1313,     0,     0,     0,  1317,
       0,  1316,  1314,     0,     0,     0,     0,     0,     0,     0,
       0,  1306,  1311,  1307,  1315,     0,     0,     0,     0,  1318,
       0,  1317,  1308,  1309,  1310,     0,     0,     0,     0,  1316,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,  1318,     0,     0,  1312,     0,  1668,     0,     0,     0,
    1327,  1317,     0,  1672,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1313,  1326,  1306,     0,  1307,     0,     0,  1314,     0,
       0,     0,  1327,     0,  1308,  1309,  1310,     0,     0,  1317,
    1315,     0,     0,     0,     0,  1306,     0,  1307,     0,     0,
       0,     0,     0,     0,  1318,  1316,  1308,  1309,  1310,     0,
       0,     0,     0,     0,     0,     0,  1673,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,  1318,     0,     0,     0,
       0,     0,     0,     0,     0,  1327,     0,     0,     0,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1318,     0,
       0,     0,     0,  1674,     0,  1317,     0,  1327,     0,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
    1306,     0,  1307,     0,     0,  1675,     0,     0,  1318,  1327,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
    1676,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
    1306,     0,  1307,     0,     0,     0,  1318,     0,     0,  1327,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,
       0,     0,     0,  1677,     0,     0,     0,  1327,     0,     0,
       0,     0,     0,     0,  1306,     0,  1307,     0,  1311,     0,
       0,     0,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,  1306,     0,  1307,     0,     0,     0,
       0,     0,  1318,     0,     0,  1308,  1309,  1310,     0,     0,
    1312,     0,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,     0,  1313,     0,  1680,
    1311,     0,     0,  1327,  1314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1315,     0,     0,     0,
       0,     0,  1311,     0,  1306,     0,  1307,     0,     0,     0,
       0,  1316,  1312,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,     0,     0,  1306,     0,  1307,  1313,
       0,     0,     0,     0,  1312,     0,  1314,  1308,  1309,  1310,
       0,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
       0,  1313,     0,     0,     0,     0,     0,     0,  1314,     0,
       0,  1317,     0,  1316,     0,     0,     0,     0,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,  1311,     0,     0,
       0,     0,     0,     0,     0,  1316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1311,     0,  1312,
       0,     0,     0,  1317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1313,     0,     0,     0,
       0,     0,     0,  1314,     0,  1317,     0,     0,     0,  1312,
       0,     0,     0,     0,     0,  1315,     0,     0,     0,     0,
       0,  1311,     0,     0,     0,     0,  1313,     0,     0,     0,
    1316,     0,     0,  1314,     0,     0,     0,     0,  1306,     0,
    1307,  1311,     0,     0,     0,  1315,     0,     0,  1318,  1308,
    1309,  1310,     0,  1312,     0,     0,     0,     0,     0,     0,
    1316,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
    1313,     0,     0,  1312,     0,  1681,     0,  1314,     0,  1327,
    1317,     0,     0,     0,     0,     0,     0,     0,     0,  1315,
    1313,     0,     0,     0,     0,     0,     0,  1314,     0,     0,
    1318,  1311,     0,     0,  1316,     0,     0,     0,     0,  1315,
    1317,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,  1318,  1311,  1316,     0,     0,  1682,     0,     0,
       0,  1327,     0,  1312,     0,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,     0,     0,     0,  1683,
    1313,     0,     0,  1327,  1317,  1312,     0,  1314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,  1313,     0,  1317,  1306,     0,  1307,     0,  1314,
       0,     0,     0,     0,  1316,     0,  1308,  1309,  1310,     0,
       0,  1315,     0,     0,     0,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,  1316,     0,     0,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,     0,  1684,     0,     0,  1318,  1327,     0,
       0,     0,     0,     0,  1317,     0,     0,     0,     0,  1685,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,     0,     0,     0,  1317,     0,  1327,     0,
       0,  1306,     0,  1307,     0,  1311,     0,     0,     0,     0,
       0,  1318,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,     0,     0,  1686,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1318,  1326,     0,     0,     0,     0,  1312,     0,     0,
       0,     0,  1327,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,  1313,     0,     0,     0,  1687,     0,
       0,  1314,  1327,     0,  1306,     0,  1307,     0,     0,     0,
       0,     0,     0,  1315,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1306,  1316,  1307,
       0,  1318,     0,     0,     0,     0,     0,     0,  1308,  1309,
    1310,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,  1318,     0,     0,     0,     0,  1688,     0,
       0,     0,  1327,     0,     0,  1689,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,     0,     0,  1317,     0,
       0,     0,  1311,     0,  1327,     0,     0,     0,     0,     0,
       0,     0,     0,  1306,     0,  1307,     0,     0,     0,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,  1306,  1312,  1307,     0,     0,     0,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,  1313,     0,  1306,     0,  1307,     0,     0,  1314,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1316,     0,     0,  1311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1306,
       0,  1307,     0,     0,     0,     0,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,  1318,     0,     0,     0,     0,
    1312,     0,     0,     0,     0,     0,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1317,  1326,  1313,     0,     0,
       0,     0,  1690,     0,  1314,     0,  1327,     0,     0,     0,
       0,  1311,     0,  1306,     0,  1307,  1315,     0,     0,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,  1316,     0,     0,  1311,     0,     0,     0,     0,     0,
       0,     0,     0,  1312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1313,     0,     0,     0,     0,     0,  1312,  1314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1315,
       0,  1317,     0,  1313,     0,     0,     0,     0,     0,     0,
    1314,     0,     0,     0,  1316,     0,     0,     0,     0,     0,
    1311,     0,  1315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1318,     0,     0,     0,     0,  1316,     0,     0,
    1311,     0,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1312,  1326,  1306,     0,  1307,     0,     0,  1693,
    1311,     0,     0,  1327,  1317,  1308,  1309,  1310,     0,  1313,
       0,     0,  1312,     0,     0,     0,  1314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1317,  1315,  1313,
       0,     0,  1312,     0,     0,     0,  1314,     0,     0,     0,
       0,     0,     0,  1316,     0,     0,  1311,     0,  1315,  1313,
       0,     0,     0,     0,     0,     0,  1314,     0,  1318,     0,
       0,     0,     0,  1316,     0,     0,     0,     0,  1315,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1312,  1326,
       0,     0,     0,  1316,     0,  1694,     0,     0,     0,  1327,
       0,     0,     0,  1317,     0,  1313,  1306,     0,  1307,     0,
    1311,     0,  1314,     0,     0,     0,     0,  1308,  1309,  1310,
       0,     0,     0,  1317,  1315,     0,     0,     0,     0,     0,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,  1316,
       0,     0,  1312,  1317,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,  1318,     0,     0,     0,  1695,  1313,
       0,     0,  1327,     0,     0,     0,  1314,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,     0,  1315,     0,
       0,  1696,     0,     0,     0,  1327,     0,     0,     0,  1317,
       0,     0,     0,  1316,  1306,     0,  1307,     0,     0,     0,
       0,     0,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1311,  1697,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1318,  1326,     0,  1317,     0,     0,     0,     0,     0,     0,
       0,  1327,  1698,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1318,  1326,     0,  1312,     0,     0,     0,     0,     0,     0,
       0,  1327,  1699,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1313,  1326,  1306,     0,  1307,     0,     0,  1314,     0,     0,
       0,  1327,     0,  1308,  1309,  1310,     0,     0,     0,  1315,
       0,     0,     0,     0,     0,     0,  1318,     0,     0,     0,
       0,     0,     0,     0,  1316,     0,     0,     0,  1700,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,
       0,     0,     0,  1311,     0,     0,  1306,  1327,  1307,     0,
       0,     0,     0,     0,     0,     0,     0,  1308,  1309,  1310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1318,     0,     0,     0,  1317,  1312,     0,     0,     0,     0,
       0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,  1313,     0,  1306,     0,  1307,  1705,     0,  1314,
       0,  1327,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,  1315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1316,     0,     0,     0,
       0,  1311,     0,  1306,     0,  1307,     0,     0,     0,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,     0,     0,  1306,     0,  1307,     0,     0,
       0,     0,     0,  1312,     0,     0,  1308,  1309,  1310,     0,
       0,     0,     0,     0,     0,     0,  1317,     0,     0,     0,
    1313,     0,     0,     0,     0,     0,     0,  1314,     0,     0,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,  1316,  1306,     0,  1307,  1706,  1311,
       0,     0,  1327,     0,     0,     0,  1308,  1309,  1310,     0,
       0,     0,     0,     0,     0,  1306,     0,  1307,     0,     0,
       0,     0,     0,     0,     0,     0,  1308,  1309,  1310,     0,
       0,  1312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1317,     0,     0,     0,  1313,     0,
    1306,     0,  1307,  1311,     0,  1314,     0,     0,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,  1315,     0,     0,
       0,     0,     0,  1318,     0,     0,     0,     0,     0,     0,
       0,     0,  1316,     0,     0,  1312,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,     0,
    1707,  1311,  1313,     0,  1327,     0,     0,     0,     0,  1314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1317,  1312,     0,     0,  1316,     0,     0,     0,
    1311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1313,     0,     0,     0,     0,     0,     0,  1314,     0,     0,
       0,  1318,  1311,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,  1312,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,  1316,     0,  1317,     0,  1708,  1313,
       0,     0,  1327,     0,  1312,     0,  1314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
       0,  1313,     0,     0,     0,     0,     0,     0,  1314,     0,
       0,     0,  1311,  1316,     0,     0,     0,     0,     0,     0,
    1315,     0,     0,     0,  1317,     0,     0,     0,     0,  1306,
       0,  1307,  1311,     0,     0,  1316,     0,     0,     0,  1318,
    1308,  1309,  1310,     0,  1312,     0,     0,     0,     0,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,  1313,     0,  1317,  1312,     0,  1709,  1311,  1314,     0,
    1327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1315,  1313,     0,     0,     0,  1317,     0,     0,  1314,     0,
       0,     0,  1306,  1318,  1307,  1316,     0,     0,     0,  1312,
    1315,     0,     0,  1308,  1309,  1310,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,  1316,  1313,     0,     0,     0,
    1712,     0,     0,  1314,  1327,  1306,     0,  1307,     0,     0,
       0,     0,     0,     0,     0,  1315,  1308,  1309,  1310,     0,
       0,  1318,     0,     0,     0,  1317,     0,     0,     0,     0,
    1316,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,  1317,     0,     0,  1715,     0,
       0,  1306,  1327,  1307,     0,     0,     0,     0,     0,     0,
    1318,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1317,  1326,  1318,     0,     0,     0,     0,  1716,     0,     0,
       0,  1327,     0,     0,  1717,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1327,  1306,     0,  1307,     0,     0,     0,
       0,     0,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,     0,     0,  1311,     0,     0,     0,
       0,     0,  1318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1318,  1326,     0,     0,     0,     0,  1312,  1718,
       0,     0,     0,  1327,     0,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,  1313,     0,     0,     0,  1719,
       0,     0,  1314,  1327,     0,     0,  1306,  1318,  1307,  1311,
       0,     0,     0,     0,  1315,     0,     0,  1308,  1309,  1310,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1316,
       0,     0,     0,     0,  1720,     0,     0,     0,  1327,     0,
       0,  1312,  1311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1313,     0,
       0,     0,     0,     0,     0,  1314,     0,     0,     0,     0,
       0,     0,     0,     0,  1312,     0,     0,  1315,     0,  1317,
       0,     0,     0,     0,     0,     0,     0,     0,  1311,     0,
       0,  1313,  1316,     0,     0,     0,     0,     0,  1314,     0,
       0,  1306,     0,  1307,     0,     0,     0,     0,     0,     0,
    1315,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
    1312,     0,     0,     0,     0,  1316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1313,     0,     0,
       0,     0,  1317,     0,  1314,     0,     0,  1306,     0,  1307,
       0,  1311,     0,     0,     0,     0,  1315,     0,  1308,  1309,
    1310,     0,     0,     0,     0,     0,     0,  1306,     0,  1307,
       0,  1316,     0,     0,     0,  1317,     0,     0,  1308,  1309,
    1310,     0,     0,  1312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1318,     0,     0,     0,
    1313,     0,     0,     0,     0,     0,     0,  1314,     0,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,  1315,
       0,  1317,     0,  1721,     0,     0,     0,  1327,     0,  1306,
       0,  1307,     0,  1311,  1316,     0,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1306,  1318,
    1307,     0,     0,     0,     0,  1312,     0,     0,     0,  1308,
    1309,  1310,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,  1313,     0,  1317,     0,  1722,     0,     0,  1314,
    1327,  1306,  1318,  1307,     0,     0,     0,     0,     0,     0,
       0,  1315,  1308,  1309,  1310,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,  1316,     0,     0,  1723,
       0,     0,     0,  1327,  1306,     0,  1307,     0,  1311,     0,
       0,     0,     0,     0,     0,  1308,  1309,  1310,  1318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
    1312,     0,     0,     0,     0,  1724,  1317,     0,     0,  1327,
       0,     0,     0,     0,  1311,     0,     0,  1313,     0,     0,
       0,     0,     0,     0,  1314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1311,     0,  1315,     0,     0,     0,
       0,  1318,     0,     0,     0,     0,  1312,     0,     0,     0,
       0,  1316,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,  1313,     0,     0,  1312,     0,  1725,     0,
    1314,     0,  1327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1315,  1313,     0,     0,     0,     0,     0,     0,
    1314,     0,     0,     0,     0,     0,  1311,  1316,     0,     0,
       0,  1317,  1315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1316,     0,     0,
       0,     0,  1306,  1318,  1307,  1311,     0,     0,  1312,     0,
       0,     0,     0,  1308,  1309,  1310,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,  1313,     0,  1317,     0,     0,
    1726,     0,  1314,     0,  1327,     0,     0,  1312,  1311,     0,
       0,     0,     0,     0,  1315,     0,     0,  1317,     0,     0,
       0,     0,     0,     0,  1313,     0,     0,     0,     0,  1316,
       0,  1314,     0,     0,     0,     0,     0,     0,     0,     0,
    1312,  1311,     0,  1315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1306,     0,  1307,  1313,  1316,     0,
       0,     0,     0,     0,  1314,  1308,  1309,  1310,  1318,     0,
       0,     0,     0,  1312,     0,     0,  1315,     0,     0,  1317,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
    1313,  1316,  1306,     0,  1307,  1727,     0,  1314,     0,  1327,
       0,     0,     0,  1308,  1309,  1310,     0,     0,  1317,  1315,
       0,     0,     0,     0,  1318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1316,     0,  1728,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1318,  1326,     0,     0,     0,     0,
       0,  1317,     0,     0,     0,  1327,     0,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,
       0,  1731,     0,     0,  1306,  1327,  1307,     0,     0,     0,
       0,     0,     0,     0,  1317,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1318,     0,     0,  1311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,
       0,     0,     0,  1732,     0,  1318,     0,  1327,     0,     0,
       0,  1312,     0,     0,     0,     0,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,  1313,     0,
       0,     0,  1733,     0,     0,  1314,  1327,     0,  1318,     0,
       0,     0,     0,     0,     0,     0,     0,  1315,     0,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,  1311,  1316,     0,  1306,  1734,  1307,     0,     0,  1327,
       0,  1318,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,  1312,     0,     0,     0,     0,  1735,  1311,
       0,     0,  1327,     0,     0,     0,     0,     0,     0,     0,
    1313,     0,  1317,     0,     0,     0,     0,  1314,     0,     0,
       0,     0,     0,  1306,     0,  1307,     0,     0,     0,  1315,
       0,  1312,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,     0,  1316,     0,     0,     0,  1313,     0,
       0,     0,     0,     0,     0,  1314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1315,  1306,     0,
    1307,  1311,     0,     0,     0,     0,     0,     0,     0,  1308,
    1309,  1310,  1316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1317,     0,     0,     0,     0,     0,
       0,     0,     0,  1312,     0,  1306,     0,  1307,     0,     0,
       0,     0,     0,     0,     0,     0,  1308,  1309,  1310,     0,
    1313,     0,     0,     0,     0,     0,     0,  1314,     0,  1318,
       0,     0,  1317,     0,     0,     0,     0,     0,     0,  1315,
       0,  1736,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,  1316,     0,     0,     0,     0,     0,
    1327,     0,     0,     0,     0,     0,  1306,     0,  1307,     0,
       0,     0,     0,     0,     0,     0,     0,  1308,  1309,  1310,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1311,     0,     0,  1306,     0,  1307,     0,     0,     0,
       0,     0,     0,     0,  1317,  1308,  1309,  1310,     0,     0,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1312,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,     0,  1737,     0,
    1313,     0,  1327,     0,     0,     0,     0,  1314,     0,  1318,
    1311,     0,     0,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,  1316,     0,  1738,     0,     0,     0,
    1327,     0,  1312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1311,     0,     0,     0,  1313,
       0,     0,     0,     0,     0,     0,  1314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
       0,  1318,     0,     0,  1317,     0,     0,  1312,     0,     0,
       0,     0,  1311,  1316,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,  1306,  1313,  1307,     0,     0,  1739,     0,
       0,  1314,  1327,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,  1315,  1312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1316,     0,
       0,  1313,     0,  1317,     0,     0,     0,     0,  1314,     0,
       0,     0,     0,  1311,     0,     0,     0,     0,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1316,     0,     0,  1306,     0,
    1307,  1311,     0,     0,     0,  1312,     0,     0,  1317,  1308,
    1309,  1310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1318,  1313,     0,     0,     0,     0,     0,     0,  1314,
       0,     0,     0,  1312,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,  1315,  1326,     0,     0,  1317,     0,     0,  1740,     0,
    1313,     0,  1327,     0,     0,     0,  1316,  1314,     0,     0,
    1306,     0,  1307,     0,     0,     0,     0,     0,     0,  1315,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
    1318,     0,     0,     0,  1316,     0,     0,     0,     0,     0,
       0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,  1306,     0,  1307,     0,  1317,  1741,     0,     0,
       0,  1327,     0,  1308,  1309,  1310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1318,     0,     0,     0,     0,
       0,     0,     0,     0,  1317,     0,     0,  1753,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,  1306,     0,  1307,
    1311,     0,     0,     0,     0,     0,  1327,     0,  1308,  1309,
    1310,     0,  1318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1755,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,  1312,  1326,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1327,     0,     0,     0,     0,     0,  1313,
    1306,     0,  1307,     0,     0,     0,  1314,     0,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,  1315,     0,
       0,     0,     0,  1318,     0,  1311,     0,     0,     0,     0,
       0,     0,     0,  1316,     0,  1758,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,     0,
       0,  1318,     0,     0,  1327,     0,     0,  1312,     0,     0,
       0,     0,     0,  1759,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,  1313,     0,     0,     0,     0,     0,
       0,  1314,  1327,  1317,     0,     0,     0,  1311,     0,  1306,
       0,  1307,     0,  1315,     0,     0,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,     0,     0,     0,  1316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1311,
       0,     0,     0,     0,     0,     0,  1313,     0,     0,     0,
       0,     0,     0,  1314,     0,     0,  1760,     0,     0,     0,
       0,     0,     0,     0,     0,  1315,     0,     0,  1317,     0,
       0,  1312,     0,     0,     0,     0,     0,     0,     0,     0,
    1316,     0,     0,     0,  1311,  1306,     0,  1307,  1313,     0,
       0,     0,     0,     0,     0,  1314,  1308,  1309,  1310,     0,
       0,     0,     0,     0,     0,     0,     0,  1315,     0,     0,
    1318,     0,     0,     0,     0,     0,  1312,     0,     0,     0,
       0,     0,  1316,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1317,  1326,     0,  1313,     0,     0,     0,  1311,     0,     0,
    1314,  1327,     0,     0,     0,     0,     0,     0,  1306,     0,
    1307,     0,  1315,     0,     0,     0,     0,     0,     0,  1308,
    1309,  1310,     0,     0,     0,     0,     0,  1316,     0,  1312,
       0,  1306,  1317,  1307,     0,     0,     0,     0,     0,     0,
       0,     0,  1308,  1309,  1310,  1318,  1313,     0,     0,     0,
       0,     0,     0,  1314,     0,     0,     0,  1763,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1315,  1326,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1327,  1317,     0,     0,
    1316,     0,     0,     0,     0,     0,  1311,     0,  1306,     0,
    1307,     0,     0,     0,     0,     0,     0,     0,     0,  1308,
    1309,  1310,     0,     0,     0,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1312,  1764,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
    1317,     0,     0,     0,  1306,  1313,  1307,     0,  1327,     0,
       0,     0,  1314,     0,     0,  1308,  1309,  1310,     0,  1318,
       0,     0,     0,     0,  1315,     0,     0,     0,     0,     0,
       0,  1765,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1316,
    1326,     0,  1311,  1306,     0,  1307,     0,     0,     0,     0,
    1327,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,     0,  1318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1312,     0,  1766,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,  1317,
       0,  1313,     0,     0,     0,  1327,     0,     0,  1314,     0,
       0,     0,     0,     0,     0,  1311,     0,     0,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,  1316,     0,     0,  1311,  1768,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,  1312,  1326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1327,     0,
       0,     0,     0,     0,  1313,     0,     0,     0,     0,     0,
    1312,  1314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1315,     0,  1317,     0,  1313,     0,     0,
       0,     0,     0,     0,  1314,  1311,     0,     0,  1316,     0,
       0,     0,     0,     0,     0,     0,  1315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1318,     0,     0,     0,
       0,  1316,     0,     0,     0,     0,     0,  1312,  1386,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,
       0,  1311,     0,  1306,  1313,  1307,     0,  1327,  1317,     0,
       0,  1314,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,  1315,     0,     0,     0,     0,     0,     0,
       0,  1317,     0,  1312,     0,     0,     0,     0,  1316,     0,
    1311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1313,     0,     0,     0,     0,     0,     0,  1314,     0,     0,
       0,     0,  1318,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,  1312,     0,  1769,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,  1316,     0,     0,     0,  1317,  1313,
       0,     0,  1306,  1327,  1307,     0,  1314,     0,     0,     0,
       0,     0,     0,  1308,  1309,  1310,     0,     0,  1315,     0,
       0,     0,     0,     0,     0,  1306,     0,  1307,     0,     0,
       0,     0,     0,  1316,     0,  1318,  1308,  1309,  1310,     0,
       0,     0,     0,     0,  1317,     0,     0,  1772,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,  1318,     0,
       0,     0,     0,     0,     0,     0,  1327,     0,     0,     0,
    1785,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,  1306,  1317,  1307,     0,     0,     0,     0,  1327,
       0,     0,     0,  1308,  1309,  1310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1306,     0,  1307,     0,     0,
       0,     0,     0,     0,     0,  1318,  1308,  1309,  1310,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,  1821,     0,
    1311,     0,     0,  1306,     0,  1307,  1327,     0,     0,     0,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1312,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,     0,  1823,  1313,
       0,  1306,  1327,  1307,     0,     0,  1314,     0,     0,     0,
    1318,     0,  1308,  1309,  1310,     0,     0,     0,  1315,     0,
       0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,  1316,     0,     0,     0,  1826,     0,  1311,
       0,  1327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1311,     0,  1306,     0,  1307,     0,     0,     0,
       0,  1312,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,  1317,     0,     0,     0,     0,  1313,     0,
       0,     0,     0,     0,  1312,  1314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1315,     0,     0,
       0,  1313,     0,     0,     0,     0,     0,     0,  1314,  1311,
       0,  1306,  1316,  1307,     0,     0,     0,     0,     0,     0,
    1315,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,     0,  1311,     0,  1306,  1316,  1307,     0,     0,     0,
       0,  1312,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1313,     0,
       0,     0,  1317,     0,  1312,  1314,     0,     0,     0,     0,
    1311,     0,     0,     0,     0,     0,     0,  1315,     0,     0,
       0,  1313,     0,     0,     0,  1317,     0,     0,  1314,     0,
    1318,     0,  1316,     0,     0,     0,     0,     0,     0,     0,
    1315,     0,  1312,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,     0,     0,  1316,     0,  1833,  1311,  1313,
    1306,  1327,  1307,     0,     0,     0,  1314,     0,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,  1315,     0,
       0,     0,  1317,     0,     0,     0,     0,     0,     0,     0,
    1312,     0,     0,  1316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1317,     0,  1313,     0,     0,
       0,     0,     0,     0,  1314,     0,     0,     0,     0,  1318,
       0,  1311,     0,     0,     0,     0,  1315,     0,     0,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,  1316,  1318,  1317,     0,     0,  1836,     0,     0,     0,
    1327,     0,     0,  1312,     0,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,     0,     0,     0,  1837,
    1313,     0,     0,  1327,     0,     0,     0,  1314,  1311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1315,
       0,  1317,     0,     0,     0,     0,     0,     0,     0,  1318,
       0,  1311,     0,     0,  1316,     0,     0,     0,     0,     0,
    1312,  1839,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,  1318,     0,     0,     0,     0,  1313,     0,     0,
    1327,     0,     0,  1312,  1314,  1319,  1320,  1321,  1322,  1323,
    1324,  1325,     0,  1326,     0,     0,  1315,     0,     0,  1846,
    1313,     0,     0,  1327,  1317,     0,     0,  1314,     0,     0,
    1318,  1316,     0,     0,     0,     0,     0,     0,     0,  1315,
       0,     0,  1847,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,  1306,  1316,  1307,     0,  1311,     0,     0,
       0,  1327,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,  1306,     0,  1307,     0,     0,  1318,     0,
       0,  1317,     0,     0,  1308,  1309,  1310,     0,     0,  1312,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,     0,  1317,  1850,  1313,     0,  1306,  1327,
    1307,     0,     0,  1314,     0,     0,     0,     0,     0,  1308,
    1309,  1310,     0,     0,     0,  1315,     0,     0,     0,     0,
       0,  1306,     0,  1307,     0,     0,     0,     0,     0,     0,
    1316,  1318,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,     0,  1851,  1306,
       0,  1307,  1327,     0,     0,     0,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,     0,     0,     0,     0,     0,
    1317,     0,     0,     0,  1306,     0,  1307,     0,  1318,     0,
       0,     0,     0,     0,     0,  1308,  1309,  1310,     0,     0,
    1852,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,  1327,
       0,     0,     0,  1853,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1306,     0,  1307,
    1311,     0,     0,     0,     0,     0,     0,  1318,  1308,  1309,
    1310,     0,  1312,     0,     0,     0,     0,     0,     0,  1854,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1313,
       0,     0,  1312,     0,     0,  1311,  1314,     0,  1327,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1315,  1313,
       0,     0,     0,     0,     0,     0,  1314,     0,  1311,     0,
    1306,     0,  1307,  1316,     0,     0,     0,  1312,  1315,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1316,  1313,     0,     0,     0,     0,     0,
    1312,  1314,     0,     0,     0,     0,  1311,     0,     0,     0,
       0,     0,     0,  1315,     0,     0,     0,  1313,     0,     0,
       0,     0,     0,  1317,  1314,     0,     0,     0,  1316,     0,
       0,  1311,     0,  1306,     0,  1307,  1315,     0,  1312,     0,
       0,     0,     0,  1317,  1308,  1309,  1310,     0,     0,     0,
       0,  1316,     0,     0,     0,  1313,  1306,     0,  1307,     0,
       0,     0,  1314,  1312,     0,     0,     0,  1308,  1309,  1310,
       0,     0,     0,     0,  1315,     0,     0,     0,  1317,  1306,
    1313,  1307,     0,     0,     0,     0,     0,  1314,     0,  1316,
    1308,  1309,  1310,     0,     0,     0,     0,     0,     0,  1315,
       0,  1317,     0,     0,     0,     0,     0,     0,     0,  1306,
       0,  1307,     0,     0,  1316,     0,     0,     0,     0,     0,
    1308,  1309,  1310,     0,     0,     0,     0,     0,     0,     0,
       0,  1306,     0,  1307,  1311,     0,     0,     0,     0,  1317,
    1318,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1318,  1326,     0,     0,  1317,     0,  1312,  1855,     0,     0,
       0,  1327,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,  1313,     0,     0,     0,  1858,     0,     0,
    1314,  1327,     0,     0,     0,  1318,     0,  1311,     0,     0,
       0,     0,  1315,     0,     0,     0,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,  1316,  1318,     0,
       0,     0,  1859,     0,     0,     0,  1327,     0,     0,  1312,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,     0,     0,  1860,  1313,     0,     0,  1327,
       0,     0,     0,  1314,     0,     0,  1318,     0,     0,     0,
    1311,     0,     0,     0,     0,  1315,     0,  1317,  1861,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,
    1316,  1318,     0,  1311,     0,     0,     0,  1327,     0,     0,
       0,     0,  1312,  1863,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,  1311,     0,     0,  1313,
       0,     0,  1327,     0,     0,  1312,  1314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
    1317,     0,  1313,  1306,     0,  1307,  1311,     0,  1312,  1314,
       0,     0,     0,  1316,  1308,  1309,  1310,     0,     0,     0,
       0,  1315,     0,     0,     0,  1313,     0,     0,  1311,     0,
       0,     0,  1314,     0,     0,     0,  1316,     0,  1312,     0,
       0,     0,     0,     0,  1315,     0,     0,     0,     0,     0,
       0,     0,  1865,     0,  1318,  1313,     0,     0,     0,  1316,
    1312,     0,  1314,  1317,     0,     0,  1864,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1315,  1326,     0,  1313,     0,  1306,
       0,  1307,     0,     0,  1314,  1327,  1317,     0,     0,  1316,
    1308,  1309,  1310,     0,     0,     0,  1315,     0,     0,     0,
       0,  1306,     0,  1307,     0,     0,     0,     0,     0,  1317,
       0,  1316,  1308,  1309,  1310,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1317,
    1306,     0,  1307,     0,     0,     0,     0,     0,  1327,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
    1306,  1317,  1307,     0,     0,     0,     0,     0,     0,     0,
       0,  1308,  1309,  1310,     0,     0,     0,     0,     0,     0,
    1318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1867,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,  1318,     0,     0,     0,     0,     0,     0,
       0,  1327,     0,     0,     0,  1868,  1319,  1320,  1321,  1322,
    1323,  1324,  1325,     0,  1326,     0,  1318,     0,     0,     0,
    1311,     0,     0,     0,  1327,     0,     0,     0,  1869,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1306,     0,
    1307,     0,     0,     0,     0,     0,  1318,  1327,     0,  1308,
    1309,  1310,  1312,     0,     0,     0,     0,     0,  1870,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1318,  1313,
       0,     0,     0,     0,     0,     0,  1314,  1327,     0,     0,
    1871,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1315,  1326,
       0,     0,     0,  1306,     0,  1307,  1311,     0,     0,  1327,
       0,     0,     0,  1316,  1308,  1309,  1310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1313,     0,     0,     0,     0,
    1312,     0,  1314,  1317,     0,     0,     0,  1311,     0,     0,
       0,     0,     0,     0,  1315,     0,     0,  1313,     0,     0,
       0,     0,     0,     0,  1314,     0,     0,  1311,     0,  1316,
       0,     0,     0,     0,     0,     0,  1315,     0,     0,  1312,
       0,  1306,     0,  1307,     0,     0,     0,     0,     0,     0,
       0,  1316,  1308,  1309,  1310,     0,  1313,     0,     0,  1312,
       0,     0,     0,  1314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1315,  1313,     0,     0,  1317,
    1306,     0,  1307,  1314,     0,     0,     0,     0,     0,     0,
    1316,  1308,  1309,  1310,     0,  1315,     0,     0,     0,     0,
       0,  1317,     0,     0,     0,     0,     0,     0,     0,     0,
    1316,     0,     0,     0,     0,  1311,     0,  1306,     0,  1307,
    1318,     0,     0,     0,     0,     0,     0,     0,  1308,  1309,
    1310,     0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1317,  1326,     0,  1872,     0,     0,     0,  1312,     0,     0,
       0,  1327,     0,     0,     0,     0,     0,     0,     0,     0,
    1317,     0,     0,     0,  1313,     0,     0,     0,     0,     0,
    1311,  1314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1315,     0,     0,     0,     0,  1306,     0,
    1307,     0,     0,     0,     0,     0,  1318,     0,  1316,  1308,
    1309,  1310,  1312,     0,     0,     0,     0,     0,     0,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,  1318,  1313,
       0,     0,     0,  1898,     0,     0,  1314,  1327,     0,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1315,  1326,
       0,     0,     0,     0,     0,  1899,     0,     0,  1317,  1327,
       0,     0,     0,  1316,     0,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1311,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,  1318,  1326,     0,
       0,     0,     0,     0,  1900,     0,     0,     0,  1327,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
    1312,     0,     0,  1317,  1901,     0,     0,  1311,  1327,     0,
       0,     0,     0,     0,     0,     0,     0,  1313,     0,     0,
       0,     0,     0,     0,  1314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1315,     0,     0,  1312,
       0,  1306,     0,  1307,  1311,     0,     0,     0,     0,     0,
       0,  1316,  1308,  1309,  1310,     0,  1313,     0,     0,     0,
       0,     0,     0,  1314,     0,  1318,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1315,  1312,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,
    1316,     0,  1902,  1313,     0,     0,  1327,     0,     0,     0,
    1314,  1317,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1306,  1315,  1307,     0,  1311,     0,     0,     0,     0,
    1318,     0,  1308,  1309,  1310,     0,     0,  1316,     0,     0,
       0,     0,  1903,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1317,  1326,     0,     0,  1306,     0,  1307,  1312,     0,     0,
       0,  1327,     0,     0,     0,  1308,  1309,  1310,     0,     0,
       0,     0,     0,     0,  1313,     0,     0,     0,     0,     0,
       0,  1314,     0,     0,     0,     0,     0,  1317,     0,     0,
       0,     0,  1306,  1315,  1307,     0,     0,     0,     0,     0,
       0,     0,     0,  1308,  1309,  1310,     0,     0,  1316,     0,
       0,     0,  1306,     0,  1307,     0,     0,     0,     0,     0,
       0,     0,     0,  1308,  1309,  1310,     0,     0,  1318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,     0,     0,  1904,     0,     0,  1317,  1327,
       0,     0,     0,     0,     0,     0,     0,  1318,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1311,     0,
    1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,
       0,     0,     0,     0,  1907,     0,     0,     0,  1327,     0,
       0,     0,     0,     0,  1318,     0,     0,     0,     0,     0,
    1312,     0,     0,     0,     0,     0,     0,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,  1313,     0,     0,
       0,  1908,     0,     0,  1314,  1327,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1315,     0,  1311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1318,     0,     0,     0,     0,
    1312,  1311,     0,     0,     0,     0,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,  1313,     0,     0,
       0,     0,  1909,     0,  1314,     0,  1327,     0,     0,     0,
       0,  1317,     0,  1312,     0,     0,  1315,     0,     0,  1311,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1313,  1316,     0,  1306,     0,  1307,     0,  1314,     0,  1311,
       0,     0,     0,     0,  1308,  1309,  1310,     0,     0,  1315,
       0,  1312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1316,     0,     0,     0,  1313,     0,
       0,  1312,  1306,     0,  1307,  1314,     0,     0,     0,     0,
       0,  1317,     0,  1308,  1309,  1310,     0,  1315,  1313,     0,
       0,     0,  1306,     0,  1307,  1314,     0,     0,     0,     0,
       0,     0,  1316,  1308,  1309,  1310,     0,  1315,     0,     0,
       0,     0,     0,     0,  1317,     0,     0,     0,  1306,     0,
    1307,     0,  1316,     0,     0,     0,     0,     0,  1318,  1308,
    1309,  1310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,  1306,  1317,  1307,     0,  1912,     0,     0,     0,  1327,
       0,     0,  1308,  1309,  1310,     0,     0,     0,     0,     0,
       0,     0,  1317,     0,     0,     0,     0,  1306,     0,  1307,
       0,     0,     0,     0,     0,     0,     0,     0,  1308,  1309,
    1310,     0,     0,     0,     0,     0,     0,  1306,  1318,  1307,
       0,     0,     0,     0,     0,     0,     0,     0,  1308,  1309,
    1310,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,     0,     0,  1915,     0,     0,     0,  1327,
       0,  1318,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1916,  1319,  1320,  1321,  1322,  1323,  1324,
    1325,     0,  1326,     0,     0,     0,     0,     0,     0,     0,
    1311,     0,  1327,     0,     0,     0,     0,     0,     0,  1318,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1917,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1318,
    1326,     0,  1312,     0,     0,     0,     0,     0,     0,  1311,
    1327,  1918,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1313,
    1326,     0,     0,     0,     0,     0,  1314,     0,     0,  1311,
    1327,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
       0,  1312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1316,     0,  1311,     0,     0,  1313,     0,
       0,  1312,     0,     0,     0,  1314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1315,  1313,     0,
       0,     0,     0,     0,     0,  1314,     0,  1312,  1311,     0,
       0,     0,  1316,     0,     0,     0,     0,  1315,     0,     0,
       0,     0,     0,  1317,  1313,     0,     0,     0,     0,     0,
       0,  1314,  1316,     0,  1311,     0,     0,     0,     0,     0,
    1312,     0,     0,  1315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1311,     0,     0,  1313,  1316,     0,
       0,     0,  1317,     0,  1314,     0,  1312,  1306,     0,  1307,
       0,     0,     0,     0,     0,     0,  1315,     0,  1308,  1309,
    1310,     0,  1317,  1313,     0,     0,  1312,  1306,     0,  1307,
    1314,  1316,     0,     0,     0,     0,     0,     0,  1308,  1309,
    1310,     0,  1315,  1313,     0,     0,     0,  1306,  1317,  1307,
    1314,     0,     0,     0,     0,     0,     0,  1316,  1308,  1309,
    1310,     0,  1315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1306,     0,  1307,     0,  1316,     0,     0,
    1318,  1317,     0,     0,  1308,  1309,  1310,     0,     0,     0,
       0,     0,  1919,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,  1306,     0,  1307,     0,     0,  1317,     0,     0,
       0,  1327,     0,  1308,  1309,  1310,     0,     0,     0,  1318,
       0,     0,     0,     0,     0,     0,     0,  1317,     0,     0,
       0,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,  1318,
    1326,     0,     0,     0,     0,     0,  1930,     0,     0,     0,
    1327,     0,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,     0,  1318,  1931,     0,     0,     0,
    1327,     0,     0,     0,     0,     0,     0,     0,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,
       0,     0,  1932,     0,     0,     0,  1327,     0,  1318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1933,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,
       0,     0,     0,     0,  1318,     0,     0,     0,     0,  1327,
       0,     0,     0,     0,  1311,     0,  1934,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1318,  1326,     0,     0,     0,     0,
       0,     0,     0,     0,  1311,  1327,  1935,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,  1312,     0,     0,     0,
       0,     0,     0,     0,  1311,  1327,     0,     0,     0,     0,
       0,     0,     0,  1313,     0,     0,  1312,     0,     0,     0,
    1314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1311,     0,  1315,  1313,     0,     0,  1312,     0,     0,     0,
    1314,     0,     0,     0,     0,     0,     0,  1316,     0,     0,
       0,     0,  1315,  1313,     0,     0,     0,     0,     0,  1311,
    1314,     0,  1312,     0,     0,     0,     0,  1316,     0,     0,
       0,     0,  1315,     0,     0,     0,     0,     0,     0,  1313,
       0,     0,     0,  1306,     0,  1307,  1314,  1316,     0,     0,
       0,  1312,     0,     0,  1308,  1309,  1310,  1317,  1315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1313,  1306,
       0,  1307,     0,  1316,     0,  1314,     0,  1317,     0,     0,
    1308,  1309,  1310,     0,     0,     0,     0,  1315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1317,  1306,     0,
    1307,     0,  1316,     0,     0,     0,     0,     0,     0,  1308,
    1309,  1310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1317,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1318,  1326,     0,     0,     0,     0,
       0,  1940,     0,     0,     0,  1327,  1941,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,  1318,  1326,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1327,  1942,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,     0,  1326,     0,     0,     0,     0,
    1318,     0,     0,     0,     0,  1327,     0,     0,     0,     0,
       0,     0,  1945,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,     0,     0,     0,     0,     0,     0,  1318,
    1311,  1327,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1947,  1319,  1320,  1321,  1322,  1323,  1324,  1325,     0,
    1326,     0,     0,     0,     0,     0,  1311,     0,     0,     0,
    1327,     0,  1312,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1313,
       0,     0,     0,     0,     0,  1311,  1314,     0,  1312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1315,     0,
       0,     0,     0,     0,     0,  1313,     0,     0,     0,     0,
       0,     0,  1314,  1316,     0,     0,     0,  1312,     0,     0,
       0,     0,     0,     0,  1315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1313,     0,     0,     0,     0,  1316,
       0,  1314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1315,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1317,     0,     0,     0,     0,  1316,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1317,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1318,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1949,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
       0,  1326,     0,     0,     0,     0,  1318,     0,     0,     0,
       0,  1327,     0,     0,     0,     0,     0,     0,     0,  1319,
    1320,  1321,  1322,  1323,  1324,  1325,     0,  1326,     0,     0,
       0,     0,     0,     0,     0,  1318,     0,  1327,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1319,  1320,
    1321,  1540,  1323,  1324,  1325,     0,  1326,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1327
};

static const yytype_int16 yycheck[] =
{
       1,     2,     3,    86,  1355,     5,    68,     8,     9,    10,
      11,   153,    13,    62,    63,    64,     5,    18,    19,    20,
      21,    22,     5,   495,    25,    26,   168,   207,     5,   171,
     238,   497,   155,    62,    63,    64,   495,   511,    39,   511,
    1333,     0,   508,   489,   490,   513,    47,    48,    49,    50,
      51,    52,   511,   512,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,   354,    67,   150,    69,     0,
      71,    72,   495,   495,     5,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,  1379,     0,   511,   511,
      91,    92,     5,   514,    95,   511,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   511,   129,   130,
     131,   495,     0,   134,    10,   511,   137,     5,   139,   140,
     506,   511,   511,   144,   145,   146,    22,   511,    79,   150,
     516,   267,   204,   205,   507,   507,   507,   511,   267,   512,
     512,   512,   511,    39,     0,   511,    79,   511,   511,     5,
      46,   511,   511,   225,    50,   291,    52,    53,    54,   511,
      56,     0,   291,    59,   511,     0,     5,     0,   511,   511,
       5,     0,     5,   511,    70,   511,     5,     5,     6,   511,
     511,     9,    78,   511,   511,   511,   511,   511,   511,   511,
     511,    79,   511,   511,   511,    91,   511,     5,     6,   511,
     511,     9,    19,    20,    21,    22,   102,   511,   104,   511,
     511,   107,   511,   511,   511,   511,   511,   511,   511,   511,
     511,   511,    39,    79,   511,   511,   122,   123,   511,    46,
     511,   127,    49,   511,   130,   511,    53,    54,    55,   511,
      79,   511,   511,    60,    79,     8,    79,    75,   511,   511,
      79,   511,   511,    70,    71,    72,    19,    20,    21,    76,
      77,    78,   511,   511,   511,     2,    83,    75,    85,   511,
     511,   511,     9,   501,     2,   511,   511,   511,   511,   511,
     511,     9,   511,   207,   101,   102,   511,   511,   105,   511,
     311,   108,   109,   511,   111,   511,   113,   511,   511,   116,
     500,   118,   511,   511,   121,   511,   511,   124,     8,   511,
      10,   512,   511,    50,   511,    52,   511,   511,   511,    19,
      20,    21,    50,    51,    52,   511,   511,   144,   145,   146,
     511,    59,   511,   511,   511,   511,   511,   511,    20,    21,
     511,    78,   511,   511,   507,    82,     8,   500,    10,   507,
      78,   511,   497,   207,    82,    92,   511,    19,    20,    21,
     512,   512,   354,    79,    92,   501,   502,   503,   504,   507,
     506,   510,   497,   510,   503,   504,   507,   506,   512,   451,
     516,   207,   512,   499,    26,   122,   123,   516,   512,   512,
     512,   512,   512,   130,   122,   123,   508,    39,   126,   127,
     207,   421,   130,   354,   512,   512,   512,   512,    50,   511,
      52,   512,   421,   512,    56,   512,   512,    59,   421,   512,
     512,   354,   512,   495,   421,    67,   512,   512,   512,   512,
     497,   512,   512,   512,   512,   510,    78,   509,  1751,   511,
     461,   515,  1813,   516,   512,   515,   512,   512,   512,   507,
      92,   508,   508,   508,   475,   238,   916,  1300,   512,   880,
    1538,  1861,    66,   734,   106,   107,   354,   734,   784,    66,
     421,    66,    -1,    -1,   302,    -1,    -1,    -1,    -1,    -1,
     122,   123,    -1,    -1,    -1,   127,    -1,   507,   421,    -1,
      -1,   511,   512,    -1,   302,     8,    -1,    10,   354,    -1,
      -1,    -1,    -1,   524,   267,    -1,    19,    20,    21,   512,
     507,    -1,    -1,    -1,   511,   354,    -1,    -1,    -1,   354,
      -1,   354,    -1,    -1,    -1,   354,    -1,    -1,   291,    -1,
      -1,    -1,     8,   421,    10,   235,   679,    -1,    -1,    -1,
      -1,   369,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   697,    -1,   507,    -1,    -1,    -1,
     511,   369,   513,   514,   460,   421,    -1,   267,    -1,    -1,
      -1,    -1,    -1,   235,   507,    -1,    -1,    -1,   511,    -1,
     513,   514,   421,    -1,   284,   267,   421,    -1,   421,    -1,
      -1,   291,   421,   421,   422,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   303,   625,   267,   627,   628,    -1,   291,
      -1,    -1,    -1,   421,   422,    -1,    -1,    -1,   318,   507,
      -1,   642,   284,   511,    -1,   513,   514,    -1,   649,   291,
      -1,    -1,   653,    -1,    -1,   656,    -1,    -1,    -1,    -1,
      -1,   303,    -1,   460,   461,   666,    -1,    -1,    -1,   670,
      -1,   507,    -1,    -1,   675,   511,   318,   513,   514,   680,
      -1,    -1,   683,    -1,    -1,    -1,    -1,   688,   368,    -1,
      -1,    -1,   500,    -1,   513,   514,   745,    -1,   513,   514,
     513,   514,    -1,    -1,   513,   514,    -1,    -1,    -1,   710,
      -1,    -1,    -1,    -1,    -1,   716,   745,   800,    -1,   507,
      -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,   729,    -1,
      -1,    -1,    -1,   734,    -1,    -1,    -1,   738,    -1,    -1,
      -1,    -1,   235,    -1,   745,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   501,   502,
     503,   504,    -1,   506,    -1,    -1,    -1,   475,    -1,    -1,
      -1,    -1,    -1,   516,   267,    -1,    -1,    -1,    -1,   235,
      -1,    -1,    -1,   784,   907,    -1,    -1,    -1,    -1,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,   800,
     942,    -1,    -1,    -1,    -1,   485,    -1,    -1,   809,    -1,
     303,   267,    -1,   814,    -1,    -1,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,   318,   506,   910,   284,    -1,
      -1,    -1,   512,   834,    -1,   291,   516,    -1,    -1,   501,
     502,   503,   504,   485,   506,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,   475,   516,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,
     512,    -1,    -1,    -1,   516,   368,    -1,    -1,    -1,    -1,
      -1,   882,    -1,   680,    -1,    -1,    -1,    -1,    -1,    -1,
     891,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   900,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   910,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     921,   922,   923,   924,   925,    -1,   927,   928,    -1,    -1,
     931,   932,    -1,   934,   935,   936,   937,   938,    -1,   940,
     941,    -1,    -1,    -1,   945,   946,    -1,    -1,    -1,   950,
     951,    -1,   953,    -1,    -1,    -1,   957,    -1,    -1,    -1,
     961,   962,    -1,    -1,    -1,    -1,   967,   968,   969,   970,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   982,   983,   984,   985,    -1,   987,   988,   989,   990,
      -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,    -1,   814,    -1,   512,
      -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1104,  1105,    -1,    -1,    -1,  1109,    -1,
    1111,    -1,    -1,    -1,  1115,  1116,  1117,  1118,    -1,    -1,
      -1,  1122,  1123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1132,    -1,  1134,  1135,  1136,  1137,    -1,    -1,    -1,
    1141,  1142,  1143,  1144,  1145,    -1,  1147,    -1,    -1,    -1,
      -1,    -1,  1153,  1154,    -1,  1156,  1157,  1158,  1159,  1160,
    1161,  1162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1175,    -1,  1177,    -1,  1301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1187,  1188,  1189,    -1,
    1191,    -1,  1193,    -1,  1195,    -1,  1197,    -1,    -1,  1200,
    1201,  1202,  1203,  1286,  1205,  1206,    -1,  1208,  1209,  1210,
      -1,    -1,    -1,    -1,    -1,  1216,    -1,  1218,  1219,  1220,
    1221,    -1,    -1,  1224,    -1,  1226,  1227,  1228,  1229,  1230,
    1231,  1232,  1233,  1234,  1235,    -1,  1359,  1238,  1239,    -1,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1258,  1259,    -1,
      -1,  1137,    -1,    -1,    -1,    -1,  1142,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1155,
      -1,  1282,    -1,    -1,    -1,  1286,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1357,    -1,  1298,  1299,  1361,
      -1,    -1,    -1,  1386,    -1,  1306,  1307,  1308,  1309,  1310,
    1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,  1326,  1327,    -1,  1204,    -1,
    1331,    -1,  1333,    -1,    -1,    -1,  1337,    -1,    -1,    -1,
    1341,    -1,  1343,  1344,  1345,  1142,    -1,  1348,    -1,    -1,
    1351,    -1,  1353,  1354,     0,    -1,    -1,    -1,  1155,     5,
      -1,    -1,    -1,  1364,    -1,  1366,    -1,  1368,    -1,    -1,
      -1,  1372,    -1,  1374,    -1,    -1,    -1,    23,  1379,    -1,
      -1,  1382,    -1,    -1,  1385,  1386,    -1,  1388,    -1,    -1,
    1391,    -1,    -1,  1394,    40,    41,    42,    43,    44,    45,
    1401,    -1,    -1,     0,    -1,    -1,    -1,  1204,  1205,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      66,  1218,    68,  1220,    -1,  1426,    23,    73,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1457,    -1,    -1,    -1,
       8,    -1,    10,    -1,    61,    62,    63,    64,    -1,    66,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,  1282,    -1,   133,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   168,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,  1538,    -1,  1540,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1586,  1587,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1617,  1618,    -1,    -1,
      -1,  1622,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1634,    -1,    -1,  1637,  1638,  1639,    -1,
    1641,    -1,  1643,    -1,  1645,    -1,  1647,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1659,  1660,
     306,  1662,    -1,    -1,    -1,  1666,    -1,    -1,    -1,    -1,
      -1,  1672,  1673,    -1,    -1,  1676,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1685,  1686,    -1,   235,  1689,    -1,
    1691,    -1,    -1,    -1,    -1,    -1,  1697,  1698,  1699,  1700,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,   306,
      -1,    -1,    -1,    19,    20,    21,  1717,    -1,    -1,   267,
      -1,    -1,    -1,     0,    -1,    -1,    -1,  1728,  1729,    -1,
      -1,    -1,    -1,    -1,    -1,  1736,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,  1745,    -1,    23,    -1,    -1,    -1,
    1751,    -1,  1753,    -1,  1755,   303,    -1,  1758,  1759,  1760,
      -1,    -1,  1763,  1764,  1765,  1766,    43,  1768,  1769,    -1,
     318,  1772,    -1,  1774,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1785,    -1,    -1,    -1,    -1,    66,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    93,    -1,    -1,    -1,
     368,    -1,    -1,  1824,    -1,    -1,  1827,    -1,  1829,    -1,
    1831,    -1,    -1,  1834,    -1,    -1,    -1,    -1,  1839,  1840,
      -1,  1842,    -1,  1844,    -1,    -1,  1847,  1848,    -1,    -1,
      -1,  1852,  1853,  1854,    -1,  1856,   133,    -1,    -1,   136,
    1861,    -1,  1863,  1864,  1865,    -1,  1867,  1868,  1869,  1870,
    1871,    -1,   149,    -1,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   207,  1903,    -1,  1905,    -1,    -1,    -1,    -1,  1910,
      -1,    -1,  1913,    -1,    -1,  1916,  1917,  1918,  1919,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1928,   574,   235,
      -1,    -1,  1933,  1934,  1935,    -1,    -1,   485,    -1,    -1,
    1941,  1942,    -1,    -1,  1945,    -1,  1947,    -1,  1949,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,   267,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   574,   284,    -1,
      -1,    -1,   628,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,   642,   303,    -1,    -1,
      -1,   647,    -1,    19,    20,    21,    -1,   653,    -1,    -1,
     656,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     666,    -1,    -1,    -1,   670,    -1,   235,    -1,    -1,   675,
      -1,   628,    -1,    -1,    -1,    -1,    -1,   683,    -1,    -1,
      -1,    -1,   688,    -1,    -1,   642,    -1,    -1,    -1,    -1,
     696,    -1,    -1,    -1,    -1,    -1,   653,    -1,   267,   656,
      -1,    -1,   368,    -1,   710,    -1,    -1,    -1,    -1,   666,
     716,    -1,    -1,   670,    -1,   284,    -1,    -1,   675,    -1,
      -1,    -1,   291,   729,    -1,     8,   683,    10,   734,    -1,
      -1,   688,   738,    -1,   303,    -1,    19,    20,    21,   745,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,   318,
      -1,    -1,    -1,   710,    -1,    -1,    19,    20,    21,   716,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   729,     8,    -1,    10,    -1,   734,   784,    -1,
      -1,   738,    -1,    -1,    19,    20,    21,    -1,   745,    -1,
      -1,    -1,    -1,    -1,   800,    -1,    -1,    -1,     8,   368,
      -1,    -1,    -1,   809,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   784,   834,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,   800,    -1,    -1,    -1,    -1,    -1,    -1,
     516,    -1,   809,    -1,    -1,    -1,    -1,    -1,    -1,   235,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   882,   834,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,   267,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   910,    -1,    -1,    -1,   284,    -1,
     916,    -1,    -1,    -1,    -1,   291,   485,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   882,    -1,   303,   497,   498,
     499,   500,   501,   502,   503,   504,   942,   506,    -1,    -1,
      -1,   947,   318,   512,    -1,    -1,   952,   516,    -1,    -1,
      -1,    -1,   235,   910,    -1,    -1,    -1,    -1,    -1,   916,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,   942,    -1,    -1,    -1,    -1,
     947,   628,   368,    -1,    -1,   952,    -1,    -1,    -1,    -1,
     235,   284,    -1,    -1,   267,   642,    -1,    -1,   291,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   653,    -1,    -1,   656,
     303,   284,    -1,    -1,    -1,   235,    -1,    -1,   291,   666,
      -1,    -1,   267,   670,    -1,   318,    -1,    -1,   675,    -1,
     303,    -1,    -1,    -1,    -1,    -1,   683,    -1,    -1,   284,
      -1,   688,    -1,    -1,    -1,   318,   291,   267,    -1,   696,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,   710,   284,    -1,    -1,    -1,    -1,   716,
      -1,   291,    -1,   318,    -1,   368,    -1,    -1,    -1,    -1,
      -1,    -1,   729,   303,    -1,    -1,    -1,   734,    -1,    -1,
      -1,   738,    -1,    -1,    -1,   368,    -1,    -1,   745,   485,
      -1,   235,    -1,    -1,    -1,    -1,    -1,  1123,    -1,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,   368,    -1,    -1,   512,     8,    -1,    10,
     516,    -1,    -1,   267,    -1,    -1,    -1,   784,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
     284,    -1,    -1,   800,    -1,    -1,  1123,   291,    19,    20,
      21,    -1,   809,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,    -1,   834,    -1,    -1,
      -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,   485,   506,    -1,    -1,    -1,    -1,    -1,   512,
      -1,    -1,    -1,   516,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,   368,   882,     8,    -1,    10,   512,
     485,    -1,    -1,   516,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,     8,   910,    10,    -1,    -1,   512,    -1,   916,
      -1,   516,    -1,    19,    20,    21,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   942,   516,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   952,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1355,
      -1,  1357,    -1,    -1,    -1,  1361,    -1,    -1,    -1,    -1,
      -1,   485,    -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1378,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,   235,    -1,    -1,    -1,   512,    -1,
      -1,    -1,   516,    -1,    -1,    -1,   267,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   284,    -1,    -1,   267,    -1,    -1,    -1,
     291,  1378,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   303,   284,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    -1,    -1,   235,    -1,    12,    13,    14,    -1,    -1,
      -1,    18,    19,    -1,    -1,    -1,  1123,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,   368,    -1,   235,
      -1,    38,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,    -1,
      -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,
      -1,   267,    69,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,   284,    86,
      87,    88,    89,    90,    -1,   291,   318,    94,    -1,    -1,
      97,    98,    -1,    -1,    -1,    -1,   103,   303,    -1,   106,
      -1,   108,    -1,   110,   111,   112,    -1,   114,    -1,    -1,
     117,    -1,   318,    -1,    -1,    -1,    -1,    -1,  1594,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,   139,    -1,   141,   142,   368,    -1,    -1,    -1,
      -1,    -1,    -1,   150,   485,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,   485,   506,    -1,    -1,    -1,    -1,
      -1,   512,    -1,    -1,    -1,   516,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,
      -1,   512,    -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1357,    -1,    -1,   485,  1361,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   497,   498,   499,   500,   501,
     502,   503,   504,  1749,   506,    -1,    -1,    -1,    -1,   485,
     512,    -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,    -1,   312,   313,   314,   315,   316,
     516,    -1,   319,   320,   321,   322,    -1,    -1,    -1,    -1,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,    -1,    -1,    -1,   341,    -1,    -1,  1813,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     457,    -1,    -1,    -1,    -1,    -1,   463,   464,   465,   466,
      -1,    -1,    -1,    -1,   471,   472,   473,    -1,    -1,   476,
     477,   478,   479,   480,   481,    -1,    -1,    -1,    -1,   486,
     487,   488,    -1,    -1,    -1,    -1,    -1,   494,   495,   496,
     497,   498,   499,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,    -1,   525,   526,
     527,   528,   529,   530,   531,    -1,    -1,    -1,   535,    -1,
     537,   538,   539,   540,   541,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   549,    -1,    -1,   552,   553,   554,   555,   556,
     557,    -1,    -1,    -1,   561,   562,   563,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   576,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     587,   588,   589,   590,   591,   592,   593,    -1,    -1,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   614,   615,   616,
     617,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,
      -1,    33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,
      -1,    43,    -1,    -1,    -1,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      -1,    -1,    64,    65,    66,    -1,    -1,    69,    70,    71,
      72,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,    -1,    -1,
      -1,   203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     222,   223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,
     232,   233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,
      -1,    -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,
      -1,    -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,
      -1,    -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,
     272,   273,   274,   275,   276,   277,    -1,    -1,   280,   281,
     282,   283,    -1,   285,    -1,   287,   288,   289,   290,    -1,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
      -1,    -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,
     312,    -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,
     322,   323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,
      -1,   333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,
      -1,   343,   344,   345,   346,   347,    -1,   349,    -1,    -1,
      -1,    -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,
     362,   363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,
      -1,    -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,
      -1,   383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,
     392,   393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,
      -1,   403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,
      -1,    -1,   414,   415,   416,   417,   418,    -1,    -1,   421,
      -1,    -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,
     432,   433,   434,   435,   436,   437,   438,   439,    -1,   441,
     442,   443,   444,   445,   446,   447,   448,    -1,    -1,   451,
      -1,   453,    -1,   455,    -1,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,    -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,
      -1,    -1,    -1,    -1,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,    -1,   497,    -1,    -1,    -1,   501,
     502,    -1,    -1,    -1,    -1,    -1,   508,   509,    -1,   511,
       3,     4,     5,   515,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,   351,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,   512,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,   351,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,   512,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,   351,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,   512,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,   512,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,   512,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,   512,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,   207,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,   500,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,   508,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,   499,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,   351,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,    -1,    -1,    -1,    -1,    -1,   501,   502,
       3,     4,     5,    -1,    -1,    -1,   509,    -1,   511,    -1,
      13,    14,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    -1,    -1,    -1,    30,    -1,    -1,
      33,    -1,    35,    -1,    37,    -1,    39,    40,    -1,    -1,
      43,    -1,    -1,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    -1,    -1,    69,    70,    71,    72,
      -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,    -1,    -1,    -1,
     203,   204,   205,    -1,    -1,   208,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   222,
     223,   224,   225,    -1,    -1,    -1,   229,    -1,    -1,   232,
     233,   234,    -1,   236,    -1,    -1,    -1,    -1,   241,    -1,
      -1,    -1,   245,   246,   247,   248,    -1,    -1,    -1,    -1,
      -1,   254,   255,   256,   257,    -1,    -1,   260,    -1,    -1,
      -1,    -1,   265,   266,    -1,   268,   269,   270,    -1,   272,
     273,   274,   275,   276,   277,    -1,    -1,   280,   281,   282,
     283,    -1,   285,    -1,   287,   288,   289,   290,    -1,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,    -1,
      -1,   304,    -1,    -1,   307,    -1,   309,    -1,    -1,   312,
      -1,    -1,    -1,    -1,    -1,    -1,   319,   320,   321,   322,
     323,   324,    -1,    -1,    -1,   328,    -1,   330,    -1,    -1,
     333,    -1,    -1,    -1,    -1,    -1,    -1,   340,   341,    -1,
     343,   344,   345,   346,   347,    -1,   349,    -1,    -1,    -1,
      -1,    -1,   355,   356,    -1,    -1,   359,   360,   361,   362,
     363,    -1,   365,    -1,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   374,   375,   376,   377,   378,    -1,    -1,    -1,    -1,
     383,   384,   385,   386,    -1,    -1,    -1,   390,    -1,   392,
     393,    -1,   395,    -1,    -1,    -1,    -1,    -1,   401,    -1,
     403,    -1,    -1,    -1,   407,    -1,    -1,   410,   411,    -1,
      -1,   414,   415,   416,   417,   418,    -1,    -1,   421,    -1,
      -1,    -1,   425,    -1,   427,   428,    -1,    -1,    -1,   432,
     433,   434,   435,   436,   437,   438,   439,    -1,   441,   442,
     443,   444,   445,   446,   447,   448,    -1,    -1,   451,    -1,
     453,    -1,   455,    -1,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
      -1,    -1,   475,    -1,   477,    -1,    -1,    -1,   481,    -1,
      -1,    -1,    -1,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,     7,    -1,    -1,    -1,    11,    12,    -1,
      -1,    -1,    -1,    17,    -1,    -1,   509,    -1,   511,    -1,
      -1,    -1,    -1,    27,    28,    29,    -1,    31,    32,    -1,
      34,    -1,    36,    -1,    38,    -1,    -1,    41,    42,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
      -1,    -1,   206,    -1,    -1,   209,   210,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,    -1,   230,   231,    -1,    -1,
      -1,    -1,    -1,   237,    -1,   239,   240,    -1,   242,   243,
     244,    -1,    -1,    -1,    -1,   249,   250,   251,   252,   253,
      -1,    -1,    -1,    -1,   258,   259,    -1,   261,   262,   263,
     264,    -1,    -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   279,    -1,    -1,    -1,    -1,
      -1,    -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   305,   306,    -1,   308,    -1,   310,   311,    -1,   313,
     314,   315,   316,   317,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   325,   326,   327,    -1,   329,    -1,   331,   332,    -1,
     334,   335,   336,   337,   338,   339,    -1,    -1,   342,    -1,
      -1,    -1,    -1,    -1,   348,    -1,   350,    -1,   352,   353,
     354,    -1,    -1,   357,   358,    -1,    -1,   235,    -1,    -1,
     364,    -1,   366,   367,    -1,    -1,    -1,   371,   372,   373,
      -1,    -1,    -1,    -1,    -1,   379,   380,   381,   382,   235,
      -1,    -1,    -1,   387,   388,   389,    -1,   391,    -1,   267,
     394,    -1,   396,   397,   398,   399,   400,    -1,   402,    -1,
     404,   405,   406,    -1,   408,   409,   284,    -1,   412,   413,
      -1,   267,    -1,   291,    -1,   419,    -1,    -1,    -1,   423,
     424,    -1,   426,    -1,    -1,   303,   430,    -1,   284,    -1,
      -1,    -1,    -1,    -1,    -1,   291,   440,    -1,    -1,    -1,
     318,    -1,    -1,    -1,    -1,   449,   450,   303,   452,    -1,
     454,    -1,   456,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,   473,
     474,    -1,   476,    -1,   478,   479,   480,    -1,   482,   483,
     484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     368,   495,   496,     7,    -1,    -1,    -1,    11,    12,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,   511,    -1,    -1,
      -1,   515,   368,    27,    28,    29,    -1,    31,    32,    -1,
      34,    -1,    36,    -1,    38,    -1,    -1,    41,    42,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,
      -1,    -1,    -1,    67,    68,    -1,    -1,    -1,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,   485,
      -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,
     516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,   201,   202,    -1,
      -1,    -1,   206,    -1,    -1,   209,   210,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,    -1,   230,   231,    -1,    -1,
      -1,    -1,    -1,   237,    -1,   239,   240,    -1,   242,   243,
     244,    -1,    -1,    -1,    -1,   249,   250,   251,   252,   253,
      -1,    -1,    -1,    -1,   258,   259,    -1,   261,   262,   263,
     264,    -1,    -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   279,    -1,    -1,    -1,    -1,
      -1,    -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   305,   306,    -1,   308,    -1,   310,   311,    -1,   313,
     314,   315,   316,   317,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   325,   326,   327,    -1,   329,    -1,   331,   332,    -1,
     334,   335,   336,   337,   338,   339,    -1,    -1,   342,    -1,
      -1,    -1,    -1,    -1,   348,    -1,   350,    -1,   352,   353,
     354,    -1,    -1,   357,   358,    -1,    -1,    -1,    -1,    -1,
     364,    -1,   366,   367,    -1,    -1,    -1,   371,   372,   373,
      -1,    -1,    -1,    -1,    -1,   379,   380,   381,   382,    -1,
      -1,    -1,    -1,   387,   388,   389,    -1,   391,    -1,    -1,
     394,    -1,   396,   397,   398,   399,   400,    -1,   402,    -1,
     404,   405,   406,    -1,   408,   409,    -1,    -1,   412,   413,
      -1,    -1,    -1,    -1,    -1,   419,    -1,    -1,    -1,   423,
     424,    -1,   426,    -1,    -1,    -1,   430,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   440,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   449,   450,    -1,   452,    -1,
     454,    -1,   456,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,   473,
     474,    17,   476,    -1,   478,   479,   480,    -1,   482,   483,
     484,    27,    28,    29,    -1,    31,    32,    -1,    34,    -1,
      36,   495,    38,    -1,    -1,    41,    42,    -1,    44,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,   511,    -1,    -1,
      -1,   515,    -1,    -1,    -1,    61,    62,    63,    -1,    -1,
      -1,    67,    68,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   200,   201,   202,    -1,    -1,    -1,
     206,    -1,    -1,   209,   210,   211,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,    -1,   230,   231,    -1,    -1,     8,    -1,
      10,   237,    -1,   239,   240,    -1,   242,   243,   244,    19,
      20,    21,    -1,   249,   250,   251,   252,   253,     8,    -1,
      10,    -1,   258,   259,    -1,   261,   262,   263,   264,    19,
      20,    21,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,   278,   279,     8,    -1,    10,    -1,    -1,    -1,
     286,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   305,
     306,    -1,   308,    -1,   310,   311,    -1,   313,   314,   315,
     316,   317,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   325,
     326,   327,    -1,   329,    -1,   331,   332,   207,   334,   335,
     336,   337,   338,   339,    -1,    -1,   342,    -1,    -1,    -1,
      -1,    -1,   348,    -1,   350,    -1,   352,   353,    -1,    -1,
      -1,   357,   358,    -1,    -1,   235,    -1,    -1,   364,    -1,
     366,   367,    -1,    -1,    -1,   371,   372,   373,    -1,    -1,
      -1,    -1,    -1,   379,   380,   381,   382,   235,    -1,    -1,
      -1,   387,   388,   389,    -1,   391,    -1,   267,   394,    -1,
     396,   397,   398,   399,   400,    -1,   402,    -1,   404,   405,
     406,    -1,   408,   409,   284,    -1,   412,   413,    -1,   267,
      -1,   291,    -1,   419,    -1,    -1,    -1,   423,   424,    -1,
     426,    -1,    -1,   303,   430,    -1,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,   440,    -1,    -1,    -1,   318,    -1,
      -1,    -1,    -1,   449,   450,   303,   452,    -1,   454,    -1,
     456,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,    -1,
     318,    -1,    -1,    -1,    -1,    -1,    -1,   473,   474,    -1,
     476,    -1,   478,   479,   480,   235,   482,   483,   484,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,   368,   495,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,    -1,    -1,   284,   511,    -1,   267,    -1,   515,
     368,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   303,   284,    -1,    -1,    -1,     8,    -1,
      10,   291,    -1,   267,    -1,    -1,    -1,    -1,   318,    19,
      20,    21,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,
     284,    -1,    -1,     8,    -1,    10,    -1,   291,   318,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,   318,    -1,    -1,    -1,   368,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,   485,    -1,    -1,   368,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,   485,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    -1,   516,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,     8,   512,    10,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,   485,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,   485,   506,    -1,   508,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   516,   497,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,
      -1,   485,   512,    -1,    -1,   235,   516,    -1,    -1,    -1,
      -1,    -1,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,    -1,   512,    -1,
     235,    -1,   516,    -1,    -1,    -1,    -1,   267,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,   235,    -1,
      -1,   291,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,   235,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,   318,    -1,
     267,    -1,     8,    -1,    10,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,   284,    -1,    -1,
     267,    -1,    -1,   318,   291,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   303,   284,    -1,    -1,
     235,    -1,    -1,    -1,   291,    -1,    -1,    -1,   368,    -1,
      -1,   318,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
     235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   318,   267,   368,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,   284,
      -1,    -1,   267,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   303,   284,
      -1,    -1,    -1,    -1,    -1,   267,   291,    -1,    -1,    -1,
      -1,   368,    -1,   318,    -1,    -1,    -1,    -1,   303,     8,
      -1,    10,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,
      19,    20,    21,   318,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,   485,   318,    -1,    19,    20,
      21,    -1,    -1,   368,    -1,    -1,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,
     485,    -1,   512,   368,    -1,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,    -1,    -1,    -1,   368,   512,   485,    -1,
      -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     497,   498,   499,   500,   501,   502,   503,   504,   485,   506,
      -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,   267,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,   284,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    19,    20,    21,    -1,
     485,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
     485,   506,   318,    -1,    -1,    -1,    -1,   512,    -1,    -1,
      -1,   516,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,   485,    -1,    -1,    -1,   512,    -1,    -1,
      -1,   516,    -1,    -1,    -1,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,   235,    -1,    -1,    -1,
     512,    -1,   368,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,   235,    -1,    -1,    -1,   267,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,   291,    -1,    -1,     8,   267,    10,    -1,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,   284,    -1,     8,    -1,    10,    -1,   318,
     291,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   368,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,     8,   235,    10,    -1,    -1,   512,    -1,    -1,    -1,
     516,    -1,    19,    20,    21,    -1,    -1,   368,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   284,    -1,    -1,     8,    -1,    10,    -1,   291,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   485,   235,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,    -1,
      -1,    -1,    -1,   512,   485,   368,    -1,   516,    -1,   267,
      -1,    -1,   235,    -1,    -1,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,   284,    -1,    -1,    -1,
      -1,   512,   235,   291,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,   303,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
     318,   284,    -1,    -1,   267,    -1,    -1,    -1,   291,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     303,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,
      -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,   235,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     368,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,
      -1,    -1,   485,   235,    -1,    -1,    -1,    -1,    -1,    -1,
     267,    -1,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,   368,    -1,   284,    -1,   512,
       8,   235,    10,   516,   291,   267,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,   368,   303,    -1,    -1,    -1,
      -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,
      -1,   318,    -1,   267,    -1,     8,    -1,    10,    -1,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
     284,    -1,    -1,    -1,    -1,    -1,   318,   291,    -1,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    19,    20,    21,    -1,    -1,    -1,   485,    -1,    -1,
      -1,   368,    -1,    -1,   318,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,    -1,   512,    -1,   368,    -1,   516,    -1,
      -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   497,   498,   499,   500,   501,   502,
     503,   504,   485,   506,   368,    -1,    -1,    -1,    -1,   512,
      -1,    -1,    -1,   516,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,     8,   267,    10,    -1,    -1,   512,
      -1,    -1,    -1,   516,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,   284,    -1,     8,    -1,    10,    -1,    -1,   291,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,   485,    -1,   512,    -1,   235,    -1,   516,
      -1,    -1,    -1,    -1,    -1,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,
     512,   485,    -1,    -1,   516,    -1,   368,    -1,    -1,   267,
      -1,    -1,   235,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,   284,    -1,   512,    -1,
      -1,    -1,   516,   291,    -1,    -1,    -1,   235,    -1,    -1,
      -1,    -1,    -1,    -1,   267,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     318,   284,    -1,     8,    -1,    10,    -1,    -1,   291,   267,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,   318,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    19,    20,    21,
     368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     318,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,   368,    -1,    -1,    -1,    -1,
     512,   235,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,   267,    10,    -1,    -1,    -1,    -1,    -1,
     368,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
     284,    -1,     8,   267,    10,    -1,    -1,   291,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,   303,
     284,    -1,    -1,    -1,     8,    -1,    10,   291,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    19,    20,    21,    -1,   303,
      -1,    -1,    -1,    -1,     8,    -1,    10,   485,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    19,    20,    21,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,
      -1,    -1,   485,    -1,   368,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,   368,    -1,    -1,   485,    -1,   512,
     235,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,     8,
      -1,    10,   267,   235,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   284,   318,    -1,    -1,    -1,    -1,    -1,   291,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
      -1,   303,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,   485,   506,    -1,    -1,    -1,   318,    -1,   512,   235,
      -1,    -1,   516,   497,   498,   499,   500,   501,   502,   503,
     504,   267,   506,   368,    -1,    -1,    -1,    -1,   512,    -1,
      -1,   235,   516,    -1,    -1,    -1,    -1,    -1,   284,    -1,
      -1,   267,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,    -1,   368,   303,   284,    -1,
      -1,    -1,     8,   267,    10,   291,    -1,    -1,    -1,    -1,
      -1,    -1,   318,    19,    20,    21,    -1,   303,    -1,    -1,
     284,    -1,     8,   267,    10,    -1,    -1,   291,    -1,    -1,
      -1,    -1,   318,    19,    20,    21,    -1,    -1,    -1,   303,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,   303,
      -1,    -1,   368,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    19,    20,    21,    -1,    -1,
     485,    -1,   368,    -1,    -1,    -1,   235,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,    -1,   368,    -1,    -1,   512,    -1,    -1,
       8,   516,    10,   485,    -1,    -1,    -1,    -1,   267,    -1,
      -1,    19,    20,    21,   368,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,   284,    -1,    -1,    -1,    -1,
     512,    -1,   291,    -1,   516,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   318,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,   485,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,   485,
     506,    -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,
     516,   497,   498,   499,   500,   501,   502,   503,   504,   368,
     506,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     516,    -1,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,   485,   506,    -1,    -1,    -1,    -1,    -1,    -1,   235,
      -1,    -1,   516,   497,   498,   499,   500,   501,   502,   503,
     504,   267,   506,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,   284,    -1,
      -1,   267,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,    -1,    -1,   303,   284,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,    -1,   318,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   318,    -1,    -1,    -1,   485,   235,    -1,    -1,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,   303,
      -1,    -1,   368,    -1,    -1,    -1,    -1,   516,    -1,   267,
      -1,    -1,   235,    -1,   318,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   368,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,    -1,    -1,     8,   235,    10,
      -1,    -1,    -1,    -1,   267,   303,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     318,   284,    -1,     8,   368,    10,    -1,    -1,   291,    -1,
     267,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,     8,
      -1,    10,    -1,    -1,   291,   318,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,   303,    -1,    -1,    -1,
     368,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,   485,
      -1,   318,    19,    20,    21,   429,    -1,    -1,    -1,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,   485,
     506,    -1,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,
     516,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,
     516,   368,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,     8,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,   516,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,    -1,
      -1,    -1,   485,    -1,   235,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,    -1,    -1,   485,    -1,
     235,    -1,    -1,   516,    -1,    -1,   267,    -1,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,   284,    -1,     8,   235,    10,    -1,   516,
     291,    -1,   267,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,   235,   284,
       8,    -1,    10,    -1,    -1,    -1,   291,   318,   267,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
     267,    -1,   291,   318,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    -1,   284,    -1,    -1,
     235,    -1,    -1,    -1,   291,    -1,    -1,   368,    -1,   318,
      -1,    -1,    -1,    -1,    -1,    -1,   303,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,   318,   267,   368,    -1,    -1,    -1,    -1,    -1,    -1,
     235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,   368,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,   303,    -1,
      -1,    -1,   267,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,   368,    -1,   318,    -1,    -1,    -1,    -1,    -1,   284,
       8,    -1,    10,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,   318,   485,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,   368,    -1,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,
     485,    -1,   235,    -1,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,   368,    -1,    -1,   485,   235,    -1,    -1,
      -1,   516,    -1,    -1,   267,    -1,    -1,    -1,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,   485,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,   516,   291,   267,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
     303,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,   516,
      -1,    -1,    -1,   291,    -1,   318,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,   235,   303,    -1,    -1,    -1,    -1,
     485,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     318,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,
      -1,   516,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,
     485,   235,    -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
     368,   506,   303,    -1,    -1,    -1,    -1,   235,    -1,    -1,
       8,   516,    10,   267,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     284,    -1,     8,   235,    10,    -1,    -1,   291,    -1,   267,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,     8,    -1,
      10,    -1,    -1,   291,   318,   267,    -1,   368,    -1,    19,
      20,    21,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,    -1,   284,    -1,     8,    -1,    10,    -1,    -1,   291,
     318,    -1,   485,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,   303,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,   368,    -1,   318,   485,    -1,    -1,
      -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
     368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,   235,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   368,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,   485,    -1,    -1,    -1,    -1,   267,
      -1,    -1,    19,    20,    21,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,
     318,    -1,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,   485,    -1,   235,
      -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,   267,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,   485,   512,   235,   284,    -1,   516,    -1,
     368,   267,    -1,   291,    -1,    -1,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,   303,    -1,    -1,   284,    -1,
     512,   235,    -1,    -1,   516,   291,    -1,   267,    -1,    -1,
     318,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   318,   267,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    -1,    19,    20,    21,    -1,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,   318,    -1,
     368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,    -1,   235,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,   368,    -1,   318,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,   235,    -1,
      -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,   368,    -1,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,   284,    -1,   512,    -1,    -1,    -1,   516,   291,
     267,    -1,    -1,     8,   368,    10,    -1,    -1,    -1,    -1,
      -1,   303,    -1,    -1,    19,    20,    21,   284,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,   318,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,   485,    -1,    -1,
      -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,   485,
      -1,    -1,    -1,    -1,   512,    -1,   368,    -1,   516,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,     8,    -1,    10,   485,   512,    -1,    -1,    -1,
     516,   368,    -1,    19,    20,    21,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,
      -1,   485,   512,    -1,    -1,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   235,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,     8,   512,    10,
      -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,
       8,    -1,    10,   485,    -1,    -1,    -1,    -1,    -1,    -1,
     303,    19,    20,    21,   267,    -1,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,   318,    -1,    -1,   485,    -1,
     512,   284,    -1,    -1,   516,    -1,    -1,    -1,   291,    -1,
     235,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
     303,    -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,
      -1,    -1,    -1,    -1,    -1,   318,    -1,     8,   235,    10,
      -1,    -1,   267,    -1,    -1,   368,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,
     267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,   368,    -1,   284,    -1,    -1,
      -1,    -1,    -1,   318,   291,    -1,     8,    -1,    10,   235,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,   318,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,   284,    -1,
      -1,    -1,    -1,    -1,   235,   291,    -1,     8,    -1,    10,
      -1,    -1,   485,    -1,    -1,    -1,    -1,   303,    19,    20,
      21,   368,    -1,    -1,    -1,   498,   499,   500,   501,   502,
     503,   504,   318,   506,    -1,    -1,   267,    -1,    -1,   512,
      -1,    -1,    -1,   516,    -1,    -1,    -1,   235,    -1,    -1,
      -1,    -1,   485,   284,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,   303,   506,    -1,    -1,    -1,    -1,    -1,   267,
      -1,    -1,   368,   516,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    19,    20,    21,    -1,    -1,    -1,
     485,    -1,    -1,    -1,   235,   303,    -1,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
     318,   506,    -1,    -1,    -1,    -1,    -1,   368,   485,    -1,
      -1,   516,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,   284,    -1,   512,    -1,    -1,    -1,   516,
     291,    -1,     8,   235,    10,    -1,    -1,    -1,    -1,    -1,
     368,    -1,   303,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,    -1,    -1,    -1,   318,    -1,   485,
      -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,   284,    -1,    -1,   267,   512,    -1,    -1,   291,
     516,    -1,    -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,
      -1,   303,   284,    -1,    -1,    -1,    -1,   368,    -1,   291,
      -1,     8,    -1,    10,   485,    -1,   318,    -1,    -1,    -1,
      -1,   303,    19,    20,    21,    -1,   267,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,   318,    -1,    -1,    -1,
       8,   512,    10,   284,    -1,   516,    -1,    -1,    -1,    -1,
     291,    19,    20,    21,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,   303,    -1,    -1,    -1,   368,    -1,    -1,    -1,
     498,   499,   500,   501,   502,   503,   504,   318,   506,    -1,
     235,    -1,    -1,     8,   512,    10,   368,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,   267,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,   485,    -1,    -1,   368,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,    -1,   303,    -1,
      -1,   512,    -1,    -1,    -1,   516,    -1,    -1,     8,   235,
      10,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   267,    -1,    -1,    -1,    -1,   498,   499,   500,   501,
     502,   503,   504,   485,   506,    -1,    -1,    -1,   284,    -1,
     512,    -1,    -1,   368,   516,   291,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,    -1,   303,    -1,    -1,
     512,    -1,    -1,    -1,   516,    -1,    -1,    -1,   235,    -1,
      -1,    -1,   318,    -1,   485,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,   235,    -1,    -1,
     267,   512,    -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,
      -1,    -1,   368,    -1,   291,    -1,    -1,    -1,    -1,   267,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
     235,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,   318,    -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    -1,   235,    -1,   303,    -1,    -1,    -1,    -1,
      -1,    -1,   267,   498,   499,   500,   501,   502,   503,   504,
     318,   506,    -1,    -1,    -1,    -1,    -1,   512,    -1,   284,
      -1,   516,     8,    -1,    10,   267,   291,    -1,    -1,    -1,
      -1,   368,    -1,    19,    20,    21,    -1,    -1,   303,    -1,
      -1,    -1,   284,    -1,     8,   235,    10,    -1,    -1,   291,
      -1,    -1,    -1,   318,    -1,    19,    20,    21,    -1,    -1,
     368,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,
      -1,    -1,    -1,    -1,    -1,    -1,   318,   267,    -1,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,   284,    -1,   512,     8,    -1,    10,
     516,   291,    -1,   368,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    -1,    -1,    -1,   368,    -1,   318,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,   485,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,     8,    -1,    10,    -1,    -1,    -1,   485,   368,   516,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,   516,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,   485,    -1,    -1,    -1,   512,    -1,   235,
      -1,   516,    -1,    -1,    -1,    -1,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,     8,    -1,    10,    -1,    -1,
     512,   235,    -1,    -1,   516,    -1,    19,    20,    21,    -1,
      -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,   284,    -1,
      -1,    -1,    -1,   267,    -1,   291,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,   303,    -1,    -1,
     284,    -1,    -1,    -1,   235,    -1,   516,   291,    -1,    -1,
      -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,   267,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,   267,    -1,
     291,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   303,    -1,    -1,   284,    -1,    -1,   235,    -1,
     267,    -1,   291,    -1,   368,    -1,    -1,   318,    -1,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,   235,    -1,    -1,   318,
     267,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,
      -1,   318,    -1,    -1,   291,    -1,    -1,   368,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,   368,
      -1,   318,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,   235,    10,   303,    -1,    -1,    -1,    -1,   485,
      -1,   368,    19,    20,    21,    -1,    -1,    -1,    -1,   318,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,   485,    -1,    -1,   267,    -1,   512,    -1,    -1,    -1,
     516,   368,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,   284,   506,     8,    -1,    10,    -1,    -1,   291,    -1,
      -1,    -1,   516,    -1,    19,    20,    21,    -1,    -1,   368,
     303,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,   485,   318,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,   485,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   516,    -1,    -1,    -1,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,   485,    -1,
      -1,    -1,    -1,   512,    -1,   368,    -1,   516,    -1,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
       8,    -1,    10,    -1,    -1,   512,    -1,    -1,   485,   516,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
       8,    -1,    10,    -1,    -1,    -1,   485,    -1,    -1,   516,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,    -1,
      -1,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,   485,    -1,    -1,    19,    20,    21,    -1,    -1,
     267,    -1,    -1,    -1,    -1,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,    -1,   284,    -1,   512,
     235,    -1,    -1,   516,   291,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,   235,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,   318,   267,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,   284,
      -1,    -1,    -1,    -1,   267,    -1,   291,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,
      -1,   368,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,   267,
      -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,   368,    -1,    -1,    -1,   267,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
     318,    -1,    -1,   291,    -1,    -1,    -1,    -1,     8,    -1,
      10,   235,    -1,    -1,    -1,   303,    -1,    -1,   485,    19,
      20,    21,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,
     318,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
     284,    -1,    -1,   267,    -1,   512,    -1,   291,    -1,   516,
     368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,
     485,   235,    -1,    -1,   318,    -1,    -1,    -1,    -1,   303,
     368,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,   485,   235,   318,    -1,    -1,   512,    -1,    -1,
      -1,   516,    -1,   267,    -1,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,   512,
     284,    -1,    -1,   516,   368,   267,    -1,   291,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,   284,    -1,   368,     8,    -1,    10,    -1,   291,
      -1,    -1,    -1,    -1,   318,    -1,    19,    20,    21,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,    -1,   512,    -1,    -1,   485,   516,    -1,
      -1,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,   516,    -1,
      -1,     8,    -1,    10,    -1,   235,    -1,    -1,    -1,    -1,
      -1,   485,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,   485,   506,    -1,    -1,    -1,    -1,   267,    -1,    -1,
      -1,    -1,   516,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,   284,    -1,    -1,    -1,   512,    -1,
      -1,   291,   516,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,   318,    10,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,   485,    -1,    -1,    -1,    -1,   512,    -1,
      -1,    -1,   516,    -1,    -1,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,    -1,    -1,   368,    -1,
      -1,    -1,   235,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,     8,   267,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,   284,    -1,     8,    -1,    10,    -1,    -1,   291,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,   485,    -1,    -1,    -1,    -1,
     267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,   368,   506,   284,    -1,    -1,
      -1,    -1,   512,    -1,   291,    -1,   516,    -1,    -1,    -1,
      -1,   235,    -1,     8,    -1,    10,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,   318,    -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,    -1,    -1,    -1,    -1,    -1,   267,   291,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,   368,    -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,
     235,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   485,    -1,    -1,    -1,    -1,   318,    -1,    -1,
     235,    -1,    -1,    -1,    -1,   498,   499,   500,   501,   502,
     503,   504,   267,   506,     8,    -1,    10,    -1,    -1,   512,
     235,    -1,    -1,   516,   368,    19,    20,    21,    -1,   284,
      -1,    -1,   267,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   368,   303,   284,
      -1,    -1,   267,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,   318,    -1,    -1,   235,    -1,   303,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,   485,    -1,
      -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,   303,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,   267,   506,
      -1,    -1,    -1,   318,    -1,   512,    -1,    -1,    -1,   516,
      -1,    -1,    -1,   368,    -1,   284,     8,    -1,    10,    -1,
     235,    -1,   291,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,   368,   303,    -1,    -1,    -1,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   318,
      -1,    -1,   267,   368,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,   485,    -1,    -1,    -1,   512,   284,
      -1,    -1,   516,    -1,    -1,    -1,   291,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,    -1,   303,    -1,
      -1,   512,    -1,    -1,    -1,   516,    -1,    -1,    -1,   368,
      -1,    -1,    -1,   318,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   497,   498,   499,   500,   501,   502,   503,   504,
     485,   506,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   516,   497,   498,   499,   500,   501,   502,   503,   504,
     485,   506,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   516,   497,   498,   499,   500,   501,   502,   503,   504,
     284,   506,     8,    -1,    10,    -1,    -1,   291,    -1,    -1,
      -1,   516,    -1,    19,    20,    21,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,    -1,
      -1,    -1,    -1,   235,    -1,    -1,     8,   516,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    -1,    -1,   368,   267,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,   284,    -1,     8,    -1,    10,   512,    -1,   291,
      -1,   516,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,
      -1,   235,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,   267,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,    -1,    -1,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,   318,     8,    -1,    10,   512,   235,
      -1,    -1,   516,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    -1,    -1,    -1,   284,    -1,
       8,    -1,    10,   235,    -1,   291,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   318,    -1,    -1,   267,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,
     512,   235,   284,    -1,   516,    -1,    -1,    -1,    -1,   291,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   368,   267,    -1,    -1,   318,    -1,    -1,    -1,
     235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,
      -1,   485,   235,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,   267,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,   318,    -1,   368,    -1,   512,   284,
      -1,    -1,   516,    -1,   267,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,
      -1,    -1,   235,   318,    -1,    -1,    -1,    -1,    -1,    -1,
     303,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,     8,
      -1,    10,   235,    -1,    -1,   318,    -1,    -1,    -1,   485,
      19,    20,    21,    -1,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,   284,    -1,   368,   267,    -1,   512,   235,   291,    -1,
     516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     303,   284,    -1,    -1,    -1,   368,    -1,    -1,   291,    -1,
      -1,    -1,     8,   485,    10,   318,    -1,    -1,    -1,   267,
     303,    -1,    -1,    19,    20,    21,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,   318,   284,    -1,    -1,    -1,
     512,    -1,    -1,   291,   516,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    19,    20,    21,    -1,
      -1,   485,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,
     318,    -1,    -1,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,   368,    -1,    -1,   512,    -1,
      -1,     8,   516,    10,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
     368,   506,   485,    -1,    -1,    -1,    -1,   512,    -1,    -1,
      -1,   516,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   516,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
      -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   498,   499,   500,   501,   502,
     503,   504,   485,   506,    -1,    -1,    -1,    -1,   267,   512,
      -1,    -1,    -1,   516,    -1,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,   284,    -1,    -1,    -1,   512,
      -1,    -1,   291,   516,    -1,    -1,     8,   485,    10,   235,
      -1,    -1,    -1,    -1,   303,    -1,    -1,    19,    20,    21,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,   318,
      -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,
      -1,   267,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,    -1,   303,    -1,   368,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,
      -1,   284,   318,    -1,    -1,    -1,    -1,    -1,   291,    -1,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
     303,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
     267,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,
      -1,    -1,   368,    -1,   291,    -1,    -1,     8,    -1,    10,
      -1,   235,    -1,    -1,    -1,    -1,   303,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,   318,    -1,    -1,    -1,   368,    -1,    -1,    19,    20,
      21,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,   303,
      -1,   368,    -1,   512,    -1,    -1,    -1,   516,    -1,     8,
      -1,    10,    -1,   235,   318,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,   485,
      10,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    19,
      20,    21,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,   284,    -1,   368,    -1,   512,    -1,    -1,   291,
     516,     8,   485,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   303,    19,    20,    21,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,   318,    -1,    -1,   512,
      -1,    -1,    -1,   516,     8,    -1,    10,    -1,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
     267,    -1,    -1,    -1,    -1,   512,   368,    -1,    -1,   516,
      -1,    -1,    -1,    -1,   235,    -1,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,    -1,   303,    -1,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,
      -1,   318,    -1,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,   284,    -1,    -1,   267,    -1,   512,    -1,
     291,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   303,   284,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,    -1,    -1,    -1,    -1,   235,   318,    -1,    -1,
      -1,   368,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    -1,     8,   485,    10,   235,    -1,    -1,   267,    -1,
      -1,    -1,    -1,    19,    20,    21,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,   284,    -1,   368,    -1,    -1,
     512,    -1,   291,    -1,   516,    -1,    -1,   267,   235,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    -1,   368,    -1,    -1,
      -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,   318,
      -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     267,   235,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,   284,   318,    -1,
      -1,    -1,    -1,    -1,   291,    19,    20,    21,   485,    -1,
      -1,    -1,    -1,   267,    -1,    -1,   303,    -1,    -1,   368,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
     284,   318,     8,    -1,    10,   512,    -1,   291,    -1,   516,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,   368,   303,
      -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,   485,   506,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,   516,    -1,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,
      -1,   512,    -1,    -1,     8,   516,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,   235,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,    -1,
      -1,    -1,    -1,   512,    -1,   485,    -1,   516,    -1,    -1,
      -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,   284,    -1,
      -1,    -1,   512,    -1,    -1,   291,   516,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,   235,   318,    -1,     8,   512,    10,    -1,    -1,   516,
      -1,   485,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,   267,    -1,    -1,    -1,    -1,   512,   235,
      -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,    -1,   368,    -1,    -1,    -1,    -1,   291,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,   303,
      -1,   267,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,   284,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,     8,    -1,
      10,   235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   267,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,   485,
      -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,
     516,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    19,    20,    21,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   267,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,    -1,   512,    -1,
     284,    -1,   516,    -1,    -1,    -1,    -1,   291,    -1,   485,
     235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,   318,    -1,   512,    -1,    -1,    -1,
     516,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
      -1,   485,    -1,    -1,   368,    -1,    -1,   267,    -1,    -1,
      -1,    -1,   235,   318,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,     8,   284,    10,    -1,    -1,   512,    -1,
      -1,   291,   516,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   303,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,
      -1,   284,    -1,   368,    -1,    -1,    -1,    -1,   291,    -1,
      -1,    -1,    -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,     8,    -1,
      10,   235,    -1,    -1,    -1,   267,    -1,    -1,   368,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   485,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,
      -1,    -1,    -1,   267,   498,   499,   500,   501,   502,   503,
     504,   303,   506,    -1,    -1,   368,    -1,    -1,   512,    -1,
     284,    -1,   516,    -1,    -1,    -1,   318,   291,    -1,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,     8,    -1,    10,    -1,   368,   512,    -1,    -1,
      -1,   516,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    -1,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,     8,    -1,    10,
     235,    -1,    -1,    -1,    -1,    -1,   516,    -1,    19,    20,
      21,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,   267,   506,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,   284,
       8,    -1,    10,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,   485,    -1,   235,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   318,    -1,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,
      -1,   485,    -1,    -1,   516,    -1,    -1,   267,    -1,    -1,
      -1,    -1,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,   284,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   516,   368,    -1,    -1,    -1,   235,    -1,     8,
      -1,    10,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,   318,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,
      -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,    -1,   431,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,   368,    -1,
      -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     318,    -1,    -1,    -1,   235,     8,    -1,    10,   284,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,
     485,    -1,    -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,
      -1,    -1,   318,   498,   499,   500,   501,   502,   503,   504,
     368,   506,    -1,   284,    -1,    -1,    -1,   235,    -1,    -1,
     291,   516,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,   318,    -1,   267,
      -1,     8,   368,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    21,   485,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,    -1,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,   303,   506,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   516,   368,    -1,    -1,
     318,    -1,    -1,    -1,    -1,    -1,   235,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
     368,    -1,    -1,    -1,     8,   284,    10,    -1,   516,    -1,
      -1,    -1,   291,    -1,    -1,    19,    20,    21,    -1,   485,
      -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,   318,
     506,    -1,   235,     8,    -1,    10,    -1,    -1,    -1,    -1,
     516,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,   368,
      -1,   284,    -1,    -1,    -1,   516,    -1,    -1,   291,    -1,
      -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,    -1,
     303,    -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,   235,   497,
     498,   499,   500,   501,   502,   503,   504,   267,   506,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,
     267,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,   368,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,   291,   235,    -1,    -1,   318,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,
      -1,   318,    -1,    -1,    -1,    -1,    -1,   267,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,    -1,
      -1,   235,    -1,     8,   284,    10,    -1,   516,   368,    -1,
      -1,   291,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,   267,    -1,    -1,    -1,    -1,   318,    -1,
     235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,
      -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,   267,    -1,   497,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,   318,    -1,    -1,    -1,   368,   284,
      -1,    -1,     8,   516,    10,    -1,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,   318,    -1,   485,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,   368,    -1,    -1,   497,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   516,    -1,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,     8,   368,    10,    -1,    -1,    -1,    -1,   516,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   485,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,   508,    -1,
     235,    -1,    -1,     8,    -1,    10,   516,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   267,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,    -1,   512,   284,
      -1,     8,   516,    10,    -1,    -1,   291,    -1,    -1,    -1,
     485,    -1,    19,    20,    21,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,   318,    -1,    -1,    -1,   512,    -1,   235,
      -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,   267,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,   284,    -1,
      -1,    -1,    -1,    -1,   267,   291,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,   291,   235,
      -1,     8,   318,    10,    -1,    -1,    -1,    -1,    -1,    -1,
     303,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,    -1,     8,   318,    10,    -1,    -1,    -1,
      -1,   267,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,
      -1,    -1,   368,    -1,   267,   291,    -1,    -1,    -1,    -1,
     235,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,   284,    -1,    -1,    -1,   368,    -1,    -1,   291,    -1,
     485,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     303,    -1,   267,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,    -1,    -1,   318,    -1,   512,   235,   284,
       8,   516,    10,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,   303,    -1,
      -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     267,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   368,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,   485,
      -1,   235,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,   318,   485,   368,    -1,    -1,   512,    -1,    -1,    -1,
     516,    -1,    -1,   267,    -1,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,   512,
     284,    -1,    -1,   516,    -1,    -1,    -1,   291,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,
      -1,   235,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,
     267,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,   485,    -1,    -1,    -1,    -1,   284,    -1,    -1,
     516,    -1,    -1,   267,   291,   498,   499,   500,   501,   502,
     503,   504,    -1,   506,    -1,    -1,   303,    -1,    -1,   512,
     284,    -1,    -1,   516,   368,    -1,    -1,   291,    -1,    -1,
     485,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,     8,   318,    10,    -1,   235,    -1,    -1,
      -1,   516,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,   485,    -1,
      -1,   368,    -1,    -1,    19,    20,    21,    -1,    -1,   267,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,    -1,   368,   512,   284,    -1,     8,   516,
      10,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
     318,   485,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,    -1,   512,     8,
      -1,    10,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     368,    -1,    -1,    -1,     8,    -1,    10,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,
      -1,    -1,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
     235,    -1,    -1,    -1,    -1,    -1,    -1,   485,    19,    20,
      21,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,   497,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,   284,
      -1,    -1,   267,    -1,    -1,   235,   291,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,   235,    -1,
       8,    -1,    10,   318,    -1,    -1,    -1,   267,   303,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   318,   284,    -1,    -1,    -1,    -1,    -1,
     267,   291,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    -1,    -1,   284,    -1,    -1,
      -1,    -1,    -1,   368,   291,    -1,    -1,    -1,   318,    -1,
      -1,   235,    -1,     8,    -1,    10,   303,    -1,   267,    -1,
      -1,    -1,    -1,   368,    19,    20,    21,    -1,    -1,    -1,
      -1,   318,    -1,    -1,    -1,   284,     8,    -1,    10,    -1,
      -1,    -1,   291,   267,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,   368,     8,
     284,    10,    -1,    -1,    -1,    -1,    -1,   291,    -1,   318,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,   235,    -1,    -1,    -1,    -1,   368,
     485,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
     485,   506,    -1,    -1,   368,    -1,   267,   512,    -1,    -1,
      -1,   516,    -1,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,   284,    -1,    -1,    -1,   512,    -1,    -1,
     291,   516,    -1,    -1,    -1,   485,    -1,   235,    -1,    -1,
      -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,   318,   485,    -1,
      -1,    -1,   512,    -1,    -1,    -1,   516,    -1,    -1,   267,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,    -1,    -1,   512,   284,    -1,    -1,   516,
      -1,    -1,    -1,   291,    -1,    -1,   485,    -1,    -1,    -1,
     235,    -1,    -1,    -1,    -1,   303,    -1,   368,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,    -1,
     318,   485,    -1,   235,    -1,    -1,    -1,   516,    -1,    -1,
      -1,    -1,   267,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,   235,    -1,    -1,   284,
      -1,    -1,   516,    -1,    -1,   267,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
     368,    -1,   284,     8,    -1,    10,   235,    -1,   267,   291,
      -1,    -1,    -1,   318,    19,    20,    21,    -1,    -1,    -1,
      -1,   303,    -1,    -1,    -1,   284,    -1,    -1,   235,    -1,
      -1,    -1,   291,    -1,    -1,    -1,   318,    -1,   267,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   420,    -1,   485,   284,    -1,    -1,    -1,   318,
     267,    -1,   291,   368,    -1,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,   303,   506,    -1,   284,    -1,     8,
      -1,    10,    -1,    -1,   291,   516,   368,    -1,    -1,   318,
      19,    20,    21,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,   368,
      -1,   318,    19,    20,    21,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,   368,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,   516,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
       8,   368,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   516,    -1,    -1,    -1,   497,   498,   499,   500,   501,
     502,   503,   504,    -1,   506,    -1,   485,    -1,    -1,    -1,
     235,    -1,    -1,    -1,   516,    -1,    -1,    -1,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,   485,   516,    -1,    19,
      20,    21,   267,    -1,    -1,    -1,    -1,    -1,   497,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,   485,   284,
      -1,    -1,    -1,    -1,    -1,    -1,   291,   516,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,   303,   506,
      -1,    -1,    -1,     8,    -1,    10,   235,    -1,    -1,   516,
      -1,    -1,    -1,   318,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
     267,    -1,   291,   368,    -1,    -1,    -1,   235,    -1,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,   235,    -1,   318,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,   267,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   318,    19,    20,    21,    -1,   284,    -1,    -1,   267,
      -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   303,   284,    -1,    -1,   368,
       8,    -1,    10,   291,    -1,    -1,    -1,    -1,    -1,    -1,
     318,    19,    20,    21,    -1,   303,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     318,    -1,    -1,    -1,    -1,   235,    -1,     8,    -1,    10,
     485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,   498,   499,   500,   501,   502,   503,   504,
     368,   506,    -1,   508,    -1,    -1,    -1,   267,    -1,    -1,
      -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     368,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,
     235,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,   485,    -1,   318,    19,
      20,    21,   267,    -1,    -1,    -1,    -1,    -1,    -1,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,   485,   284,
      -1,    -1,    -1,   512,    -1,    -1,   291,   516,    -1,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,   303,   506,
      -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,   368,   516,
      -1,    -1,    -1,   318,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,
     498,   499,   500,   501,   502,   503,   504,   485,   506,    -1,
      -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
     267,    -1,    -1,   368,   512,    -1,    -1,   235,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,   267,
      -1,     8,    -1,    10,   235,    -1,    -1,    -1,    -1,    -1,
      -1,   318,    19,    20,    21,    -1,   284,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,   485,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   303,   267,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,
     318,    -1,   512,   284,    -1,    -1,   516,    -1,    -1,    -1,
     291,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,   303,    10,    -1,   235,    -1,    -1,    -1,    -1,
     485,    -1,    19,    20,    21,    -1,    -1,   318,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
     368,   506,    -1,    -1,     8,    -1,    10,   267,    -1,    -1,
      -1,   516,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,
      -1,   291,    -1,    -1,    -1,    -1,    -1,   368,    -1,    -1,
      -1,    -1,     8,   303,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,   318,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,   368,   516,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,
     498,   499,   500,   501,   502,   503,   504,    -1,   506,    -1,
      -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,
     267,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,   284,    -1,    -1,
      -1,   512,    -1,    -1,   291,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,   235,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,
     267,   235,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,   284,    -1,    -1,
      -1,    -1,   512,    -1,   291,    -1,   516,    -1,    -1,    -1,
      -1,   368,    -1,   267,    -1,    -1,   303,    -1,    -1,   235,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,   318,    -1,     8,    -1,    10,    -1,   291,    -1,   235,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,   303,
      -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,   284,    -1,
      -1,   267,     8,    -1,    10,   291,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    19,    20,    21,    -1,   303,   284,    -1,
      -1,    -1,     8,    -1,    10,   291,    -1,    -1,    -1,    -1,
      -1,    -1,   318,    19,    20,    21,    -1,   303,    -1,    -1,
      -1,    -1,    -1,    -1,   368,    -1,    -1,    -1,     8,    -1,
      10,    -1,   318,    -1,    -1,    -1,    -1,    -1,   485,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,     8,   368,    10,    -1,   512,    -1,    -1,    -1,   516,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   368,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,     8,   485,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,   516,
      -1,   485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   497,   498,   499,   500,   501,   502,   503,
     504,    -1,   506,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,   485,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,   485,
     506,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,   235,
     516,   497,   498,   499,   500,   501,   502,   503,   504,   284,
     506,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,   235,
     516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
      -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   318,    -1,   235,    -1,    -1,   284,    -1,
      -1,   267,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,   284,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    -1,   267,   235,    -1,
      -1,    -1,   318,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,   368,   284,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   318,    -1,   235,    -1,    -1,    -1,    -1,    -1,
     267,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,    -1,    -1,   284,   318,    -1,
      -1,    -1,   368,    -1,   291,    -1,   267,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    19,    20,
      21,    -1,   368,   284,    -1,    -1,   267,     8,    -1,    10,
     291,   318,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,   303,   284,    -1,    -1,    -1,     8,   368,    10,
     291,    -1,    -1,    -1,    -1,    -1,    -1,   318,    19,    20,
      21,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,   318,    -1,    -1,
     485,   368,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,     8,    -1,    10,    -1,    -1,   368,    -1,    -1,
      -1,   516,    -1,    19,    20,    21,    -1,    -1,    -1,   485,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,    -1,
      -1,    -1,   498,   499,   500,   501,   502,   503,   504,   485,
     506,    -1,    -1,    -1,    -1,    -1,   512,    -1,    -1,    -1,
     516,    -1,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,    -1,   485,   512,    -1,    -1,    -1,
     516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,
      -1,    -1,   512,    -1,    -1,    -1,   516,    -1,   485,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     497,   498,   499,   500,   501,   502,   503,   504,    -1,   506,
      -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,   516,
      -1,    -1,    -1,    -1,   235,    -1,   497,   498,   499,   500,
     501,   502,   503,   504,   485,   506,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   516,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,   267,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   284,    -1,    -1,   267,    -1,    -1,    -1,
     291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,    -1,   303,   284,    -1,    -1,   267,    -1,    -1,    -1,
     291,    -1,    -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    -1,   303,   284,    -1,    -1,    -1,    -1,    -1,   235,
     291,    -1,   267,    -1,    -1,    -1,    -1,   318,    -1,    -1,
      -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,   284,
      -1,    -1,    -1,     8,    -1,    10,   291,   318,    -1,    -1,
      -1,   267,    -1,    -1,    19,    20,    21,   368,   303,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,     8,
      -1,    10,    -1,   318,    -1,   291,    -1,   368,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   368,     8,    -1,
      10,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,   500,
     501,   502,   503,   504,   485,   506,    -1,    -1,    -1,    -1,
      -1,   512,    -1,    -1,    -1,   516,   497,   498,   499,   500,
     501,   502,   503,   504,   485,   506,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   516,   497,   498,   499,   500,
     501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,    -1,
     485,    -1,    -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   485,
     235,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   497,   498,   499,   500,   501,   502,   503,   504,    -1,
     506,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
     516,    -1,   267,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,
      -1,    -1,    -1,    -1,    -1,   235,   291,    -1,   267,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,   291,   318,    -1,    -1,    -1,   267,    -1,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,   318,
      -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,   318,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   368,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     485,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   497,   498,   499,   500,   501,   502,   503,   504,
      -1,   506,    -1,    -1,    -1,    -1,   485,    -1,    -1,    -1,
      -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,
     499,   500,   501,   502,   503,   504,    -1,   506,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   485,    -1,   516,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   498,   499,
     500,   501,   502,   503,   504,    -1,   506,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   516
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     7,    11,    12,    17,    27,    28,    29,    31,    32,
      34,    36,    38,    41,    42,    44,    45,    46,    61,    62,
      63,    67,    68,    73,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,   200,   201,   202,   206,   209,
     210,   211,   220,   226,   227,   228,   230,   231,   237,   239,
     240,   242,   243,   244,   249,   250,   251,   252,   253,   258,
     259,   261,   262,   263,   264,   271,   278,   279,   286,   305,
     306,   308,   310,   311,   313,   314,   315,   316,   317,   325,
     326,   327,   329,   331,   332,   334,   335,   336,   337,   338,
     339,   342,   348,   350,   352,   353,   354,   357,   358,   364,
     366,   367,   371,   372,   373,   379,   380,   381,   382,   387,
     388,   389,   391,   394,   396,   397,   398,   399,   400,   402,
     404,   405,   406,   408,   409,   412,   413,   419,   423,   424,
     426,   430,   440,   449,   450,   452,   454,   456,   473,   474,
     476,   478,   479,   480,   482,   483,   484,   495,   496,   511,
     515,   518,   520,   521,   522,   528,   529,   530,   561,   562,
     563,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   579,   581,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   599,   600,   601,   602,   603,   604,   605,
     606,   607,   608,   609,   610,   611,   612,   613,   614,   615,
     616,   617,   618,   619,   620,   621,   622,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,   644,   645,
     646,   647,   648,   649,   650,   651,   652,   653,   654,   655,
     656,   657,   658,   659,   660,   661,   662,   663,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,   691,   692,   693,   694,   695,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,     3,     4,     5,    13,    14,    15,
      16,    18,    22,    23,    24,    25,    26,    30,    33,    35,
      37,    39,    40,    43,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    64,    65,
      66,    69,    70,    71,    72,    74,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   203,
     204,   205,   208,   212,   213,   214,   215,   216,   217,   218,
     219,   221,   222,   223,   224,   225,   229,   232,   233,   234,
     236,   241,   245,   246,   247,   248,   254,   255,   256,   257,
     260,   265,   266,   268,   269,   270,   272,   273,   274,   275,
     276,   277,   280,   281,   282,   283,   285,   287,   288,   289,
     290,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   304,   307,   309,   312,   319,   320,   321,   322,   323,
     324,   328,   330,   333,   340,   341,   343,   344,   345,   346,
     347,   349,   355,   356,   359,   360,   361,   362,   363,   365,
     370,   374,   375,   376,   377,   378,   383,   384,   385,   386,
     390,   392,   393,   395,   401,   403,   407,   410,   411,   414,
     415,   416,   417,   418,   421,   425,   427,   428,   432,   433,
     434,   435,   436,   437,   438,   439,   441,   442,   443,   444,
     445,   446,   447,   448,   451,   453,   455,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   475,   477,   481,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   501,   502,   509,   511,   528,
     529,   530,   534,   538,   553,   554,   555,   556,   557,   558,
     559,   560,   511,   548,   549,   553,   553,   511,   528,   564,
     553,   548,   549,   511,   546,   553,   511,   542,   553,   511,
     542,   542,   553,   542,   542,   553,   511,   542,   543,   553,
     511,   543,   553,   543,   553,   511,   543,   546,   553,   528,
     529,   530,   553,   511,   547,   553,   542,   542,   511,   543,
     546,   547,   553,   528,   528,   528,   511,   528,   529,   531,
     533,   528,   528,   543,   546,   553,   553,   553,   543,   553,
     511,   546,   547,   548,   549,   553,   511,   548,   553,   546,
     547,   548,   549,   543,   546,   543,   546,   543,   553,   511,
     546,   547,   553,   553,   511,   551,   552,   553,   511,   546,
     547,   548,   553,   543,   553,   511,   528,   530,   544,   545,
     553,   528,   530,   544,   545,   528,   530,   544,   545,   528,
     530,   544,   545,   553,   592,   593,   594,   547,   533,   542,
     553,   543,   546,   543,   553,   543,   553,   528,   542,   528,
     543,   553,   543,   553,   511,   543,   546,   547,   548,   549,
     550,   553,   553,   553,   553,   548,   549,   543,   553,   543,
     511,   537,   542,   553,   542,   542,   542,   542,   546,   511,
     547,   548,   549,   553,   529,   530,   542,   553,   542,   553,
     542,   553,   553,   553,   543,   553,   543,   546,   553,   542,
     553,   546,   543,   553,   511,   542,   547,   553,   546,   547,
     542,   543,   553,   543,   553,   542,   553,   542,   543,   553,
     542,   553,   543,   553,   542,   553,   553,   543,   553,   542,
     553,   543,   553,   553,   553,   543,   546,   547,   548,   549,
     546,   547,   548,   549,   543,   553,   548,   546,   547,   548,
     528,   553,   511,   546,   548,   549,   553,   553,   529,   530,
     553,   578,   529,   553,   542,   542,   553,   553,   542,   542,
     580,   543,   553,   543,   553,   543,   553,   528,   529,   530,
     511,   537,   542,   553,     0,   513,   519,   354,   522,   354,
       5,     6,     9,    75,   302,   369,   421,   422,   507,   523,
     527,   207,   500,     5,     6,     9,    75,   302,   369,   421,
     422,   500,   514,   522,   522,   511,   511,   511,   528,   530,
     511,   511,   511,   511,   553,   542,   542,   511,   542,   542,
     542,   511,   511,   542,   542,   542,   542,   511,   511,   511,
     511,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   511,   511,   511,   511,   542,   511,   511,   511,
     511,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   511,   511,   543,   546,   543,   511,
     542,   511,   542,   542,   542,   511,   511,   511,   511,   542,
     542,   542,   511,   511,   547,   548,   553,   542,   542,   542,
     542,   542,   511,   542,   511,   511,   511,   511,   542,   542,
     542,   511,   511,   511,   511,   511,   542,   511,   542,   542,
     542,   542,   542,   511,   511,   511,   511,   511,   511,   511,
     511,   511,   511,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   511,   542,   511,   542,   553,
     542,   542,   542,   542,   542,   542,   542,   511,   511,   511,
     542,   511,   542,   511,   542,   511,   542,   511,   542,   542,
     511,   511,   511,   511,   511,   511,   511,   542,   511,   511,
     511,   542,   542,   542,   542,   542,   511,   542,   511,   511,
     511,   511,   542,   542,   511,   542,   511,   511,   511,   511,
     511,   511,   511,   511,   511,   511,   528,   530,   511,   511,
     542,   511,   511,   511,   511,   511,   511,   511,   511,   511,
     511,   542,   542,   542,   542,   542,   542,   542,   511,   511,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   542,   542,   542,   542,   542,   542,   542,   542,
     542,   542,   511,   553,   489,   490,   509,   535,   536,   537,
     539,   553,   528,   529,   530,   553,     5,   421,   507,   511,
     523,   524,   525,   526,     5,   421,     8,    10,    19,    20,
      21,   235,   267,   284,   291,   303,   318,   368,   485,   498,
     499,   500,   501,   502,   503,   504,   506,   516,   548,   549,
     553,   497,   528,   511,   553,   546,   553,   497,   512,   543,
     553,   497,   553,   497,   207,   500,   543,   553,   207,   547,
     553,   497,   553,   497,   500,   238,   501,   511,   532,   528,
     507,   497,   429,   553,   497,   553,   497,   553,   497,   551,
     552,   553,   497,   553,   497,   544,   545,   553,   497,   511,
     550,   553,   497,   537,   553,   368,   497,   553,   497,   543,
     553,   207,   532,   553,   497,   553,   553,   512,   512,   512,
     537,   368,   518,   354,   553,   553,   553,   553,   553,   553,
     508,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   522,    79,   553,   553,   528,   507,   553,   553,   529,
     530,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   497,   553,   553,
     553,   553,   546,   553,   543,   546,   553,   553,   553,   553,
     553,   553,   553,   543,   546,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   543,   546,   543,   553,   553,
     553,   553,   553,   553,   543,   553,   553,   543,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   543,   537,   510,   497,   510,
     501,   512,   497,   515,   351,   512,   540,   541,   553,   507,
     524,   523,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   512,   512,   553,   512,   540,   512,
     553,   512,   553,   553,   553,   553,   207,   207,   553,   512,
     553,   553,   553,   560,   499,   512,   533,   533,   553,   553,
     553,   512,   512,   553,   553,   512,   512,   528,   530,   512,
     540,   512,   553,   512,   537,   553,   553,   207,   207,   553,
     553,   512,   497,   508,   512,   512,   512,   512,   512,   512,
     512,   512,   512,   512,   497,   512,   512,   497,   497,   497,
     512,   497,   512,   497,   512,   497,   512,   497,   512,   512,
     512,   512,   512,   512,   512,   512,   512,   512,   512,   497,
     497,   553,   497,   512,   512,   512,   497,   512,   512,   512,
     512,   512,   497,   497,   512,   512,   497,   512,   512,   512,
     512,   512,   512,   512,   512,   497,   497,   512,   512,   497,
     512,   497,   512,   512,   512,   512,   512,   497,   497,   497,
     497,   512,   512,   512,   512,   512,   512,   512,   512,   512,
     512,   512,   512,   512,   512,   512,   512,   497,   512,   512,
     512,   512,   512,   512,   512,   512,   512,   512,   497,   497,
     512,   512,   512,   512,   512,   512,   497,   512,   512,   512,
     512,   512,   512,   510,   535,   499,   515,   497,   508,   511,
     512,   497,   515,   497,   512,   497,   553,   553,   497,   497,
     431,   528,   512,   497,   497,   497,   497,   512,   497,   497,
     553,   553,   497,   553,   507,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   497,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   508,   508,   528,   540,   553,   553,
     553,   553,   553,   238,   553,   553,   553,   553,   553,   553,
     553,   508,   553,   512,   497,   512,   512,   497,   512,   497,
     512,   497,   512,   512,   497,   512,   512,   512,   553,   497,
     497,   512,   497,   512,   497,   512,   512,   497,   497,   512,
     512,   512,   497,   497,   497,   512,   497,   512,   512,   512,
     512,   497,   512,   497,   497,   420,   560,   497,   497,   497,
     497,   497,   508,   553,   553,   553,   553,   553,   553,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   539,   553,
     553,   553,   553,   553,   553,   553,   553,   553,   512,   512,
     512,   512,   512,   497,   512,   497,   512,   512,   512,   512,
     497,   512,   512,   497,   512,   512,   497,   497,   497,   497,
     553,   553,   553,   553,   553,   553,   553,   553,   497,   512,
     512,   512,   512,   497,   497,   497,   553,   553,   553,   553,
     512,   497,   497,   553,   553,   497,   553,   497,   553,   497,
     553
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
        case 4:

    {
		linenumber++;
		column=0;
		addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
	;}
    break;

  case 13:

    {
		if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
			errorcode = COMPERR_FUNCTIONGOTO;
			return -1;
		}
		//
		//check if name of label is already used by a function, subroutine or another label
		if (symtableaddress[(yyvsp[(1) - (1)].number)] != -1) {
			errorcode = COMPERR_LABELREDEFINED;
			return -1;
		}
		symtableaddress[(yyvsp[(1) - (1)].number)] = wordOffset;
		symtableaddresstype[(yyvsp[(1) - (1)].number)] = ADDRESSTYPE_LABEL;
	;}
    break;

  case 24:

    {
		addIntOp(OP_PUSHINT, 0);
		addOp(OP_STACKSWAP);
	;}
    break;

  case 25:

    {
		varnumber[nvarnumber++] = (yyvsp[(1) - (1)].number);
	;}
    break;

  case 32:

    {
			args[numargs] = varnumber[--nvarnumber]; argstype[numargs] = ARGSTYPEVALUE; numargs++;
			//printf("functionvariable %i %i %i\n", args[numargs-1], argstype[numargs-1],numargs);
		;}
    break;

  case 37:

    {
		addIntOp(OP_PUSHINT, numberoflists);		// number of lists (y dim)
		addIntOp(OP_PUSHINT, listlenmax);			// maximum number of expressions (x dim)
#ifdef DEBUG
	fprintf(stderr, "listlenmax %d\n", listlenmax);
#endif
		numberoflists = 0;
		listlenmax=0;
	;}
    break;

  case 38:

    {
		addIntOp(OP_PUSHINT, listlen);
		if (listlen>listlenmax) listlenmax=listlen;
		listlen = 0;
		numberoflists = 1;
	;}
    break;

  case 39:

    {numberoflists = 1; ;}
    break;

  case 40:

    {numberoflists++;;}
    break;

  case 41:

    {
		addIntOp(OP_PUSHINT, listlen);
		if (listlen>listlenmax) listlenmax=listlen;
		listlen = 0;
	;}
    break;

  case 42:

    { listlen = 1; ;}
    break;

  case 43:

    {listlen++;;}
    break;

  case 44:

    {
		addIntOp(OP_PUSHINT, listlen);
		if (listlen>listlenmax) listlenmax=listlen;
		listlen = 0;
	;}
    break;

  case 45:

    { listlen = 1; ;}
    break;

  case 46:

    {listlen++;;}
    break;

  case 47:

    { listlen = 1; ;}
    break;

  case 48:

    {listlen++;;}
    break;

  case 50:

    { addIntOp(OP_VAR_REF, varnumber[--nvarnumber]); ;}
    break;

  case 80:

    {
		addIntOp(OP_ARR_GET, varnumber[--nvarnumber]);
	;}
    break;

  case 82:

    {
		addOp(OP_ADD);
	;}
    break;

  case 83:

    {
		addOp(OP_CONCATENATE);
	;}
    break;

  case 84:

    {
		addOp(OP_BINARYAND);
	;}
    break;

  case 85:

    {
		addOp(OP_MUL);
	;}
    break;

  case 86:

    {
		// function call with arguments
		addIntOp(OP_PUSHINT, listlen); //push number of arguments passed to compare with FUNCTION definition
		addIntOp(OP_CALLFUNCTION, varnumber[--nvarnumber]);
		addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
	;}
    break;

  case 87:

    {
		// function call without arguments
		addIntOp(OP_PUSHINT, 0); //push number of arguments passed to compare with FUNCTION definition
		addIntOp(OP_CALLFUNCTION, varnumber[--nvarnumber]);
		addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
	;}
    break;

  case 88:

    { addIntOp(OP_PUSHINT, 0xff000000); ;}
    break;

  case 89:

    { addIntOp(OP_PUSHINT, 0xff0000ff); ;}
    break;

  case 90:

    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 91:

    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 92:

    { addIntOp(OP_PUSHINT, 0x00); ;}
    break;

  case 93:

    { addIntOp(OP_PUSHINT, 0xff00ffff); ;}
    break;

  case 94:

    { addIntOp(OP_PUSHINT, 0xff000080); ;}
    break;

  case 95:

    { addIntOp(OP_PUSHINT, 0xff008080); ;}
    break;

  case 96:

    { addIntOp(OP_PUSHINT, 0xff008000); ;}
    break;

  case 97:

    { addIntOp(OP_PUSHINT, 0xff808080); ;}
    break;

  case 98:

    { addIntOp(OP_PUSHINT, 0xffb03d00); ;}
    break;

  case 99:

    { addIntOp(OP_PUSHINT, 0xff800080); ;}
    break;

  case 100:

    { addIntOp(OP_PUSHINT, 0xff800000); ;}
    break;

  case 101:

    { addIntOp(OP_PUSHINT, 0xff808000); ;}
    break;

  case 102:

    { addIntOp(OP_PUSHINT, 0xff00ff00); ;}
    break;

  case 103:

    { addIntOp(OP_PUSHINT, 0xffa4a4a4); ;}
    break;

  case 104:

    { addIntOp(OP_PUSHINT, MOUSEBUTTON_CENTER); ;}
    break;

  case 105:

    { addIntOp(OP_PUSHINT, MOUSEBUTTON_DOUBLECLICK); ;}
    break;

  case 106:

    { addIntOp(OP_PUSHINT, MOUSEBUTTON_LEFT); ;}
    break;

  case 107:

    { addIntOp(OP_PUSHINT, MOUSEBUTTON_NONE); ;}
    break;

  case 108:

    { addIntOp(OP_PUSHINT, MOUSEBUTTON_RIGHT); ;}
    break;

  case 109:

    { addIntOp(OP_PUSHINT, 0xffff6600); ;}
    break;

  case 110:

    { addIntOp(OP_PUSHINT, OSTYPE_ANDROID); ;}
    break;

  case 111:

    { addIntOp(OP_PUSHINT, OSTYPE_LINUX); ;}
    break;

  case 112:

    { addIntOp(OP_PUSHINT, OSTYPE_MACINTOSH); ;}
    break;

  case 113:

    { addIntOp(OP_PUSHINT, OSTYPE_WINDOWS); ;}
    break;

  case 114:

    { addIntOp(OP_PUSHINT, 0xffff00ff); ;}
    break;

  case 115:

    { addIntOp(OP_PUSHINT, 0xffff0000); ;}
    break;

  case 116:

    { addIntOp(OP_PUSHINT, SLICE_ALL); ;}
    break;

  case 117:

    { addIntOp(OP_PUSHINT, SLICE_PAINT); ;}
    break;

  case 118:

    { addIntOp(OP_PUSHINT, SLICE_SPRITE); ;}
    break;

  case 119:

    { addIntOp(OP_PUSHINT, T_ARRAY); ;}
    break;

  case 120:

    { addIntOp(OP_PUSHINT, T_FLOAT); ;}
    break;

  case 121:

    { addIntOp(OP_PUSHINT, T_INT); ;}
    break;

  case 122:

    { addIntOp(OP_PUSHINT, T_MAP); ;}
    break;

  case 123:

    { addIntOp(OP_PUSHINT, T_REF); ;}
    break;

  case 124:

    { addIntOp(OP_PUSHINT, T_STRING); ;}
    break;

  case 125:

    { addIntOp(OP_PUSHINT, T_UNASSIGNED); ;}
    break;

  case 126:

    { addIntOp(OP_PUSHINT, 0xffffffff); ;}
    break;

  case 127:

    { addIntOp(OP_PUSHINT, 0xffffff00); ;}
    break;

  case 128:

    {
		addIntOp(OP_PUSHINT, ERROR_ARGUMENTCOUNT);
	;}
    break;

  case 129:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYELEMENT);
	;}
    break;

  case 130:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYEVEN);
	;}
    break;

  case 131:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYEXPR);
	;}
    break;

  case 132:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYINDEX);
	;}
    break;

  case 133:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYINDEXMISSING);
	;}
    break;

  case 134:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYLENGTH2D);
	;}
    break;

  case 135:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYNITEMS);
	;}
    break;

  case 136:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYSIZELARGE);
	;}
    break;

  case 137:

    {
		addIntOp(OP_PUSHINT, ERROR_ARRAYSIZESMALL);
	;}
    break;

  case 138:

    {
		addIntOp(OP_PUSHINT, ERROR_ASINACOSRANGE);
	;}
    break;

  case 139:

    {
		addIntOp(OP_PUSHINT, ERROR_BOOLEANCONV);
	;}
    break;

  case 140:

    {
		addIntOp(OP_PUSHINT, ERROR_DBCOLNO);
	;}
    break;

  case 141:

    {
		addIntOp(OP_PUSHINT, ERROR_DBCONNNUMBER);
	;}
    break;

  case 142:

    {
		addIntOp(OP_PUSHINT, ERROR_DBNOTOPEN);
	;}
    break;

  case 143:

    {
		addIntOp(OP_PUSHINT, ERROR_DBNOTSET);
	;}
    break;

  case 144:

    {
		addIntOp(OP_PUSHINT, ERROR_DBNOTSETROW);
	;}
    break;

  case 145:

    {
		addIntOp(OP_PUSHINT, ERROR_DBOPEN);
	;}
    break;

  case 146:

    {
		addIntOp(OP_PUSHINT, ERROR_DBQUERY);
	;}
    break;

  case 147:

    {
		addIntOp(OP_PUSHINT, ERROR_DBSETNUMBER);
	;}
    break;

  case 148:

    {
		addIntOp(OP_PUSHINT, ERROR_DIVZERO);
	;}
    break;

  case 149:

    {
		addIntOp(OP_PUSHINT, ERROR_DOWNLOAD);
	;}
    break;

  case 150:

    {
		addIntOp(OP_PUSHINT, ERROR_ENVELOPEMAX);
	;}
    break;

  case 151:

    {
		addIntOp(OP_PUSHINT, ERROR_ENVELOPEODD);
	;}
    break;

  case 152:

    {
		addIntOp(OP_PUSHINT, ERROR_EXPECTEDARRAY);
	;}
    break;

  case 153:

    {
		addIntOp(OP_PUSHINT, ERROR_EXPECTEDSOUND);
	;}
    break;

  case 154:

    {
		addIntOp(OP_PUSHINT, ERROR_FILENOTOPEN);
	;}
    break;

  case 155:

    {
		addIntOp(OP_PUSHINT, ERROR_FILENUMBER);
	;}
    break;

  case 156:

    {
		addIntOp(OP_PUSHINT, ERROR_FILEOPEN);
	;}
    break;

  case 157:

    {
		addIntOp(OP_PUSHINT, ERROR_FILEOPERATION);
	;}
    break;

  case 158:

    {
		addIntOp(OP_PUSHINT, ERROR_FILERESET);
	;}
    break;

  case 159:

    {
		addIntOp(OP_PUSHINT, ERROR_FILEWRITE);
	;}
    break;

  case 160:

    {
		addIntOp(OP_PUSHINT, ERROR_FOLDER);
	;}
    break;

  case 161:

    {
		addIntOp(OP_PUSHINT, ERROR_FREEDB);
	;}
    break;

  case 162:

    {
		addIntOp(OP_PUSHINT, ERROR_FREEDBSET);
	;}
    break;

  case 163:

    {
		addIntOp(OP_PUSHINT, ERROR_FREEFILE);
	;}
    break;

  case 164:

    {
		addIntOp(OP_PUSHINT, ERROR_FREENET);
	;}
    break;

  case 165:

    {
		addIntOp(OP_PUSHINT, ERROR_HARMONICLIST);
	;}
    break;

  case 166:

    {
		addIntOp(OP_PUSHINT, ERROR_HARMONICNUMBER);
	;}
    break;

  case 167:

    {
		addIntOp(OP_PUSHINT, ERROR_IMAGEFILE);
	;}
    break;

  case 168:

    {
		addIntOp(OP_PUSHINT, ERROR_IMAGERESOURCE);
	;}
    break;

  case 169:

    {
		addIntOp(OP_PUSHINT, ERROR_IMAGESAVETYPE);
	;}
    break;

  case 170:

    {
		addIntOp(OP_PUSHINT, ERROR_IMAGESCALE);
	;}
    break;

  case 171:

    {
		addIntOp(OP_PUSHINT, ERROR_INFINITY);
	;}
    break;

  case 172:

    {
		addIntOp(OP_PUSHINT, ERROR_INTEGERRANGE);
	;}
    break;

  case 173:

    {
		addIntOp(OP_PUSHINT, ERROR_INVALIDKEYNAME);
	;}
    break;

  case 174:

    {
		addIntOp(OP_PUSHINT, ERROR_INVALIDPROGNAME);
	;}
    break;

  case 175:

    {
		addIntOp(OP_PUSHINT, ERROR_INVALIDRESOURCE);
	;}
    break;

  case 176:

    {
		addIntOp(OP_PUSHINT, ERROR_LOGRANGE);
	;}
    break;

  case 177:

    {
		addIntOp(OP_PUSHINT, ERROR_LONGRANGE);
	;}
    break;

  case 178:

    {
		addIntOp(OP_PUSHINT, ERROR_MAXRECURSE);
	;}
    break;

  case 179:

    {
		addIntOp(OP_PUSHINT, ERROR_NETACCEPT);
	;}
    break;

  case 180:

    {
		addIntOp(OP_PUSHINT, ERROR_NETBIND);
	;}
    break;

  case 181:

    {
		addIntOp(OP_PUSHINT, ERROR_NETCONN);
	;}
    break;

  case 182:

    {
		addIntOp(OP_PUSHINT, ERROR_NETHOST);
	;}
    break;

  case 183:

    {
		addIntOp(OP_PUSHINT, ERROR_NETNONE);
	;}
    break;

  case 184:

    {
		addIntOp(OP_PUSHINT, ERROR_NETREAD);
	;}
    break;

  case 185:

    {
		addIntOp(OP_PUSHINT, ERROR_NETSOCK);
	;}
    break;

  case 186:

    {
		addIntOp(OP_PUSHINT, ERROR_NETSOCKNUMBER);
	;}
    break;

  case 187:

    {
		addIntOp(OP_PUSHINT, ERROR_NETSOCKOPT);
	;}
    break;

  case 188:

    {
		addIntOp(OP_PUSHINT, ERROR_NETWRITE);
	;}
    break;

  case 189:

    {
		addIntOp(OP_PUSHINT, ERROR_NEXTNOFOR);
	;}
    break;

  case 190:

    {
		addIntOp(OP_PUSHINT, ERROR_NONE);
	;}
    break;

  case 191:

    {
		addIntOp(OP_PUSHINT, ERROR_NOSUCHFUNCTION);
	;}
    break;

  case 192:

    {
		addIntOp(OP_PUSHINT, ERROR_NOSUCHLABEL);
	;}
    break;

  case 193:

    {
		addIntOp(OP_PUSHINT, ERROR_NOSUCHSUBROUTINE);
	;}
    break;

  case 194:

    {
		addIntOp(OP_PUSHINT, ERROR_NOTARRAY);
	;}
    break;

  case 195:

    {
		addIntOp(OP_PUSHINT, ERROR_NOTIMPLEMENTED);
	;}
    break;

  case 196:

    {
		addIntOp(OP_PUSHINT, ERROR_NUMBERCONV);
	;}
    break;

  case 197:

    {
		addIntOp(OP_PUSHINT, ERROR_NUMBEREXPR);
	;}
    break;

  case 198:

    {
		addIntOp(OP_PUSHINT, ERROR_ONEDIMENSIONAL);
	;}
    break;

  case 199:

    {
		addIntOp(OP_PUSHINT, ERROR_ONERRORSUB);
	;}
    break;

  case 200:

    {
		addIntOp(OP_PUSHINT, ERROR_PENWIDTH);
	;}
    break;

  case 201:

    {
		addIntOp(OP_PUSHINT, ERROR_PERMISSION);
	;}
    break;

  case 202:

    {
		addIntOp(OP_PUSHINT, ERROR_POLYPOINTS);
	;}
    break;

  case 203:

    {
		addIntOp(OP_PUSHINT, ERROR_PRINTERNOTOFF);
	;}
    break;

  case 204:

    {
		addIntOp(OP_PUSHINT, ERROR_PRINTERNOTON);
	;}
    break;

  case 205:

    {
		addIntOp(OP_PUSHINT, ERROR_PRINTEROPEN);
	;}
    break;

  case 206:

    {
		addIntOp(OP_PUSHINT, ERROR_RADIX);
	;}
    break;

  case 207:

    {
		addIntOp(OP_PUSHINT, ERROR_RADIXSTRING);
	;}
    break;

  case 208:

    {
		addIntOp(OP_PUSHINT, ERROR_REFNOTASSIGNED);
	;}
    break;

  case 209:

    {
		addIntOp(OP_PUSHINT, ERROR_RGB);
	;}
    break;

  case 210:

    {
		addIntOp(OP_PUSHINT, ERROR_SERIALPARAMETER);
	;}
    break;

  case 211:

    {
		addIntOp(OP_PUSHINT, ERROR_SETTINGMAXKEYS);
	;}
    break;

  case 212:

    {
		addIntOp(OP_PUSHINT, ERROR_SETTINGMAXLEN);
	;}
    break;

  case 213:

    {
		addIntOp(OP_PUSHINT, ERROR_SETTINGSGETACCESS);
	;}
    break;

  case 214:

    {
		addIntOp(OP_PUSHINT, ERROR_SETTINGSSETACCESS);
	;}
    break;

  case 215:

    {
		addIntOp(OP_PUSHINT, ERROR_SLICESIZE);
	;}
    break;

  case 216:

    {
		addIntOp(OP_PUSHINT, ERROR_SOUNDERROR);
	;}
    break;

  case 217:

    {
		addIntOp(OP_PUSHINT, ERROR_SOUNDFILE);
	;}
    break;

  case 218:

    {
		addIntOp(OP_PUSHINT, ERROR_SOUNDFILEFORMAT);
	;}
    break;

  case 219:

    {
		addIntOp(OP_PUSHINT, ERROR_SOUNDLENGTH);
	;}
    break;

  case 220:

    {
		addIntOp(OP_PUSHINT, ERROR_SOUNDNOTSEEKABLE);
	;}
    break;

  case 221:

    {
		addIntOp(OP_PUSHINT, ERROR_SOUNDRESOURCE);
	;}
    break;

  case 222:

    {
		addIntOp(OP_PUSHINT, ERROR_SPRITENA);
	;}
    break;

  case 223:

    {
		addIntOp(OP_PUSHINT, ERROR_SPRITENUMBER);
	;}
    break;

  case 224:

    {
		addIntOp(OP_PUSHINT, ERROR_SPRITESLICE);
	;}
    break;

  case 225:

    {
		addIntOp(OP_PUSHINT, ERROR_SQRRANGE);
	;}
    break;

  case 226:

    {
		addIntOp(OP_PUSHINT, ERROR_STACKUNDERFLOW);
	;}
    break;

  case 227:

    {
		addIntOp(OP_PUSHINT, ERROR_STRING2NOTE);
	;}
    break;

  case 228:

    {
		addIntOp(OP_PUSHINT, ERROR_STRINGCONV);
	;}
    break;

  case 229:

    {
		addIntOp(OP_PUSHINT, ERROR_STRINGEXPR);
	;}
    break;

  case 230:

    {
		addIntOp(OP_PUSHINT, ERROR_STRINGMAXLEN);
	;}
    break;

  case 231:

    {
		addIntOp(OP_PUSHINT, ERROR_STRSTART);
	;}
    break;

  case 232:

    {
		addIntOp(OP_PUSHINT, ERROR_TOOMANYSOUNDS);
	;}
    break;

  case 233:

    {
		addIntOp(OP_PUSHINT, ERROR_UNEXPECTEDRETURN);
	;}
    break;

  case 234:

    {
		addIntOp(OP_PUSHINT, ERROR_UNSERIALIZEFORMAT);
	;}
    break;

  case 235:

    {
		addIntOp(OP_PUSHINT, ERROR_VARCIRCULAR);
	;}
    break;

  case 236:

    {
		addIntOp(OP_PUSHINT, ERROR_VARNOTASSIGNED);
	;}
    break;

  case 237:

    {
		addIntOp(OP_PUSHINT, ERROR_VARNULL);
	;}
    break;

  case 238:

    {
		addIntOp(OP_PUSHINT, ERROR_WAVEFORMLOGICAL);
	;}
    break;

  case 239:

    {
		addIntOp(OP_PUSHINT, ERROR_WAVOBSOLETE);
	;}
    break;

  case 240:

    {
		addIntOp(OP_PUSHINT, WARNING_ARRAYELEMENT);
	;}
    break;

  case 241:

    {
		addIntOp(OP_PUSHINT, WARNING_BOOLEANCONV);
	;}
    break;

  case 242:

    {
		addIntOp(OP_PUSHINT, WARNING_INTEGERRANGE);
	;}
    break;

  case 243:

    {
		addIntOp(OP_PUSHINT, WARNING_LONGRANGE);
	;}
    break;

  case 244:

    {
		addIntOp(OP_PUSHINT, WARNING_NUMBERCONV);
	;}
    break;

  case 245:

    {
		addIntOp(OP_PUSHINT, WARNING_REFNOTASSIGNED);
	;}
    break;

  case 246:

    {
		addIntOp(OP_PUSHINT, WARNING_SOUNDERROR);
	;}
    break;

  case 247:

    {
		addIntOp(OP_PUSHINT, WARNING_SOUNDFILEFORMAT);
	;}
    break;

  case 248:

    {
		addIntOp(OP_PUSHINT, WARNING_SOUNDLENGTH);
	;}
    break;

  case 249:

    {
		addIntOp(OP_PUSHINT, WARNING_SOUNDNOTSEEKABLE);
	;}
    break;

  case 250:

    {
		addIntOp(OP_PUSHINT, WARNING_START);
	;}
    break;

  case 251:

    {
		addIntOp(OP_PUSHINT, WARNING_STRING2NOTE);
	;}
    break;

  case 252:

    {
		addIntOp(OP_PUSHINT, WARNING_STRINGCONV);
	;}
    break;

  case 253:

    {
		addIntOp(OP_PUSHINT, WARNING_VARNOTASSIGNED);
	;}
    break;

  case 254:

    {
		addIntOp(OP_PUSHINT, WARNING_WAVOBSOLETE);
	;}
    break;

  case 255:

    { addIntOp(OP_PUSHINT, (yyvsp[(1) - (1)].number)); ;}
    break;

  case 256:

    {
		if(isfinite((yyvsp[(1) - (1)].floatnum))){
			addFloatOp(OP_PUSHFLOAT, (yyvsp[(1) - (1)].floatnum));
		}else{
			errorcode = COMPERR_NUMBERTOOLARGE;
			return -1;
		}
	;}
    break;

  case 257:

    {
		 // accept/eat unary plus only for numbers
		 addIntOp(OP_PUSHINT, (yyvsp[(2) - (2)].number));
	;}
    break;

  case 258:

    {
		// accept/eat unary plus only for numbers
		if(isfinite((yyvsp[(2) - (2)].floatnum))){
			addFloatOp(OP_PUSHFLOAT, (yyvsp[(2) - (2)].floatnum));
		}else{
			errorcode = COMPERR_NUMBERTOOLARGE;
			return -1;
		}
	;}
    break;

  case 259:

    {
		addOp(OP_NEGATE);
	;}
    break;

  case 260:

    {
		addOp(OP_SUB);
	;}
    break;

  case 261:

    {
		addOp(OP_MOD);
	;}
    break;

  case 262:

    {
		addOp(OP_MOD);
	;}
    break;

  case 263:

    {
		/* expression% is actually a percentage */
		addIntOp(OP_PUSHINT, 100);
		addOp(OP_DIV);
	;}
    break;

  case 264:

    {
		addOp(OP_INTDIV);
	;}
    break;

  case 265:

    {
		addOp(OP_DIV);
	;}
    break;

  case 266:

    { addOp(OP_EX); ;}
    break;

  case 267:

    { addOp(OP_BINARYOR); ;}
    break;

  case 268:

    { addOp(OP_BITSHIFTL); ;}
    break;

  case 269:

    { addOp(OP_BITSHIFTR); ;}
    break;

  case 270:

    { addOp(OP_BINARYNOT); ;}
    break;

  case 271:

    {
		addOp(OP_AND);
	;}
    break;

  case 272:

    {
		addOp(OP_OR);
	;}
    break;

  case 273:

    { addOp(OP_XOR); ;}
    break;

  case 274:

    { addOp(OP_NOT); ;}
    break;

  case 275:

    { addOp(OP_EQUAL); ;}
    break;

  case 276:

    { addOp(OP_NEQUAL); ;}
    break;

  case 277:

    { addOp(OP_LT); ;}
    break;

  case 278:

    { addOp(OP_GT); ;}
    break;

  case 279:

    { addOp(OP_GTE); ;}
    break;

  case 280:

    { addOp(OP_LTE); ;}
    break;

  case 281:

    {
		// a[b,c]++ - pushing value before to stack
		int v = varnumber[--nvarnumber];
		addOp(OP_STACKDUP2);			// save indexes
		addIntOp(OP_ARR_GET, v);		// get current value
		addOp(OP_STACKDUP);				// duplicate (1 to save and 1 to increment)
		addOp(OP_STACKSAVE);			// save original
		addIntOp(OP_PUSHINT,1);			// add 1
		addOp(OP_ADD);
		addIntOp(OP_ARR_SET, v);		// assign new value
		addOp(OP_STACKUNSAVE);			// put original value on the stack
	;}
    break;

  case 282:

    {
		// a[b,c]-- - pushing value before to stack
		int v = varnumber[--nvarnumber];
		addOp(OP_STACKDUP2);			// save indexes
		addIntOp(OP_ARR_GET, v);		// get current value
		addOp(OP_STACKDUP);				// duplicate (1 to save and 1 to increment)
		addOp(OP_STACKSAVE);			// save original
		addIntOp(OP_PUSHINT,-1);		// subtract 1
		addOp(OP_ADD);
		addIntOp(OP_ARR_SET, v);		// assign new value
		addOp(OP_STACKUNSAVE);			// put original value on the stack
	;}
    break;

  case 283:

    {
		// ++a[b,c] - pushing value after to stack
		int v = varnumber[--nvarnumber];
		addOp(OP_STACKDUP2);			// save indexes
		addIntOp(OP_ARR_GET, v);		// get current value
		addIntOp(OP_PUSHINT,1);			// add 1
		addOp(OP_ADD);
		addOp(OP_STACKDUP);				// duplicate (1 to set 1 to stack)
		addOp(OP_STACKSAVE);			// save 1 to stack
		addIntOp(OP_ARR_SET, v);		// assign new value
		addOp(OP_STACKUNSAVE);			// put new value on the stack
	;}
    break;

  case 284:

    {
		// --a[b,c] - pushing value after to stack
		int v = varnumber[--nvarnumber];
		addOp(OP_STACKDUP2);			// save indexes
		addIntOp(OP_ARR_GET, v);		// get current value
		addIntOp(OP_PUSHINT,-1);		// subtract 1
		addOp(OP_ADD);
		addOp(OP_STACKDUP);				// duplicate (1 to set 1 to stack)
		addOp(OP_STACKSAVE);			// save 1 to stack
		addIntOp(OP_ARR_SET, v);		// assign new value
		addOp(OP_STACKUNSAVE);			// put new value on the stack
	;}
    break;

  case 285:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
		addIntOp(OP_PUSHINT,1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
	;}
    break;

  case 286:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
		addIntOp(OP_PUSHINT,-1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
	;}
    break;

  case 287:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_PUSHINT,1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
	;}
    break;

  case 288:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_PUSHINT,-1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
	;}
    break;

  case 289:

    { addOp(OP_INT); ;}
    break;

  case 290:

    { addOp(OP_FLOAT); ;}
    break;

  case 291:

    { addOp(OP_LENGTH); ;}
    break;

  case 292:

    { addOp(OP_ASC); ;}
    break;

  case 293:

    {
		addIntOp(OP_PUSHINT, 1);	// start
		addIntOp(OP_PUSHINT, 0);	// case sens flag
		addOp(OP_INSTR);
	;}
    break;

  case 294:

    {
		addIntOp(OP_PUSHINT, 0);	// case sens flag
		addOp(OP_INSTR);
	 ;}
    break;

  case 295:

    { addOp(OP_INSTR); ;}
    break;

  case 296:

    {
		addIntOp(OP_PUSHINT, 1);	//start
		addOp(OP_INSTRX);
	;}
    break;

  case 297:

    { addOp(OP_INSTRX); ;}
    break;

  case 298:

    { addOp(OP_CEIL); ;}
    break;

  case 299:

    { addOp(OP_FLOOR); ;}
    break;

  case 300:

    { addOp(OP_SIN); ;}
    break;

  case 301:

    { addOp(OP_COS); ;}
    break;

  case 302:

    { addOp(OP_TAN); ;}
    break;

  case 303:

    { addOp(OP_ASIN); ;}
    break;

  case 304:

    { addOp(OP_ACOS); ;}
    break;

  case 305:

    { addOp(OP_ATAN); ;}
    break;

  case 306:

    { addOp(OP_DEGREES); ;}
    break;

  case 307:

    { addOp(OP_RADIANS); ;}
    break;

  case 308:

    { addOp(OP_LOG); ;}
    break;

  case 309:

    { addOp(OP_LOGTEN); ;}
    break;

  case 310:

    { addOp(OP_SQR); ;}
    break;

  case 311:

    { addOp(OP_EXP); ;}
    break;

  case 312:

    { addOp(OP_ABS); ;}
    break;

  case 313:

    { addOp(OP_RAND); ;}
    break;

  case 314:

    { addFloatOp(OP_PUSHFLOAT, 3.14159265358979323846); ;}
    break;

  case 315:

    {
		addIntOp(OP_PUSHINT, 0);
		addOp(OP_EOF);
	;}
    break;

  case 316:

    { addOp(OP_EOF); ;}
    break;

  case 317:

    { addOp(OP_EXISTS); ;}
    break;

  case 318:

    { addOp(OP_YEAR); ;}
    break;

  case 319:

    { addOp(OP_MONTH); ;}
    break;

  case 320:

    { addOp(OP_DAY); ;}
    break;

  case 321:

    { addOp(OP_HOUR); ;}
    break;

  case 322:

    { addOp(OP_MINUTE); ;}
    break;

  case 323:

    { addOp(OP_SECOND); ;}
    break;

  case 324:

    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 325:

    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 326:

    {
		addIntOp(OP_PUSHINT, 0);
		addOp(OP_SIZE);
	;}
    break;

  case 327:

    { addOp(OP_SIZE); ;}
    break;

  case 328:

    {
		addIntOp(OP_PUSHINT, 0x00);
		addOp(OP_KEYPRESSED);
	;}
    break;

  case 329:

    { addOp(OP_KEYPRESSED); ;}
    break;

  case 330:

    { addOp(OP_KEY); ;}
    break;

  case 331:

    { addOp(OP_MOUSEX); ;}
    break;

  case 332:

    { addOp(OP_MOUSEY); ;}
    break;

  case 333:

    { addOp(OP_MOUSEB); ;}
    break;

  case 334:

    { addOp(OP_CLICKX); ;}
    break;

  case 335:

    { addOp(OP_CLICKY); ;}
    break;

  case 336:

    { addOp(OP_CLICKB); ;}
    break;

  case 337:

    { addOp(OP_PIXEL); ;}
    break;

  case 338:

    {
		addIntOp(OP_PUSHINT,255);	// a
		addOp(OP_RGB);
	;}
    break;

  case 339:

    {
		addOp(OP_RGB);
	;}
    break;

  case 340:

    { addOp(OP_GETCOLOR); ;}
    break;

  case 341:

    { addOp(OP_GETBRUSHCOLOR); ;}
    break;

  case 342:

    { addOp(OP_GETPENWIDTH); ;}
    break;

  case 343:

    { addOp(OP_SPRITECOLLIDE); ;}
    break;

  case 344:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_SPRITECOLLIDE); ;}
    break;

  case 345:

    { addOp(OP_SPRITEX); ;}
    break;

  case 346:

    { addOp(OP_SPRITEY); ;}
    break;

  case 347:

    { addOp(OP_SPRITEH); ;}
    break;

  case 348:

    { addOp(OP_SPRITEW); ;}
    break;

  case 349:

    { addOp(OP_SPRITEV); ;}
    break;

  case 350:

    { addOp(OP_SPRITER); ;}
    break;

  case 351:

    { addOp(OP_SPRITES); ;}
    break;

  case 352:

    { addOp(OP_SPRITEO); ;}
    break;

  case 353:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_DBROW);
	;}
    break;

  case 354:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_DBROW);
	;}
    break;

  case 355:

    {
		addOp(OP_DBROW);
	;}
    break;

  case 356:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBINT); ;}
    break;

  case 357:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBINT); ;}
    break;

  case 358:

    {
		addOp(OP_DBINT); ;}
    break;

  case 359:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBFLOAT); ;}
    break;

  case 360:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBFLOAT); ;}
    break;

  case 361:

    {
		addOp(OP_DBFLOAT); ;}
    break;

  case 362:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBNULL); ;}
    break;

  case 363:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBNULL); ;}
    break;

  case 364:

    {
		addOp(OP_DBNULL); ;}
    break;

  case 365:

    { addOp(OP_LASTERROR); ;}
    break;

  case 366:

    { addOp(OP_LASTERRORLINE); ;}
    break;

  case 367:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETDATA); ;}
    break;

  case 368:

    { addOp(OP_NETDATA); ;}
    break;

  case 369:

    { addOp(OP_PORTIN); ;}
    break;

  case 370:

    {
		addIntOp(OP_PUSHINT, 0); // case sens flag
		addOp(OP_COUNT);
	 ;}
    break;

  case 371:

    { addOp(OP_COUNT); ;}
    break;

  case 372:

    { addOp(OP_COUNTX); ;}
    break;

  case 373:

    { addOp(OP_OSTYPE); ;}
    break;

  case 374:

    { addOp(OP_MSEC); ;}
    break;

  case 375:

    { addOp(OP_TEXTWIDTH); ;}
    break;

  case 376:

    { addOp(OP_TEXTBOXWIDTH); ;}
    break;

  case 377:

    { addOp(OP_TEXTHEIGHT); ;}
    break;

  case 378:

    { addOp(OP_TEXTBOXHEIGHT); ;}
    break;

  case 379:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_READBYTE); ;}
    break;

  case 380:

    { addOp(OP_READBYTE); ;}
    break;

  case 381:

    { addOp(OP_FREEDB); ;}
    break;

  case 382:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_FREEDBSET);
	;}
    break;

  case 383:

    { addOp(OP_FREEDBSET); ;}
    break;

  case 384:

    { addOp(OP_FREEFILE); ;}
    break;

  case 385:

    { addOp(OP_FREENET); ;}
    break;

  case 386:

    { addIntOp(OP_PUSHINT, VERSIONSIGNATURE); ;}
    break;

  case 387:

    {
		addIntOp(OP_PUSHINT,-1);	// no default
		addOp(OP_CONFIRM);
	;}
    break;

  case 388:

    {
		addOp(OP_CONFIRM);
	;}
    break;

  case 389:

    {
		addIntOp(OP_PUSHINT,2);	// radix
		addOp(OP_FROMRADIX);
	;}
    break;

  case 390:

    {
		addIntOp(OP_PUSHINT,16);	// radix
		addOp(OP_FROMRADIX);
	;}
    break;

  case 391:

    {
		addIntOp(OP_PUSHINT,8);	// radix
		addOp(OP_FROMRADIX);
	;}
    break;

  case 392:

    {
		addOp(OP_FROMRADIX);
	;}
    break;

  case 393:

    {
		addIntOp(OP_PUSHINT,strtoul((yyvsp[(1) - (1)].string), NULL, 2));
		if(errno==ERANGE){
			errorcode = COMPERR_NUMBERTOOLARGE;
			return -1;
		}
		//addStringOp(OP_PUSHSTRING, $1);
		//addIntOp(OP_PUSHINT,2);	// radix
		//addOp(OP_FROMRADIX);
	;}
    break;

  case 394:

    {
		addIntOp(OP_PUSHINT,strtoul((yyvsp[(1) - (1)].string), NULL, 16));
		if(errno==ERANGE){
			errorcode = COMPERR_NUMBERTOOLARGE;
			return -1;
		}
		//addStringOp(OP_PUSHSTRING, $1);
		//addIntOp(OP_PUSHINT,16);	// radix
		//addOp(OP_FROMRADIX);
	;}
    break;

  case 395:

    {
		addIntOp(OP_PUSHINT,strtoul((yyvsp[(1) - (1)].string), NULL, 8));
		if(errno==ERANGE){
			errorcode = COMPERR_NUMBERTOOLARGE;
			return -1;
		}
		//addStringOp(OP_PUSHSTRING, $1);
		//addIntOp(OP_PUSHINT,8);	// radix
		//addOp(OP_FROMRADIX);
	;}
    break;

  case 396:

    { addOp(OP_WAVLENGTH); ;}
    break;

  case 397:

    { addOp(OP_WAVPOS); ;}
    break;

  case 398:

    { addOp(OP_WAVSTATE); ;}
    break;

  case 399:

    {
		addOp(OP_TYPEOF);
	;}
    break;

  case 400:

    {
		addOp(OP_SOUNDPLAYER);
	;}
    break;

  case 401:

    {
		addIntOp(OP_PUSHINT, 2);	// 2 columns
		addIntOp(OP_PUSHINT, 1);	// 1 row
		addOp(OP_LIST2ARRAY);
		addOp(OP_SOUNDPLAYER);
	;}
    break;

  case 402:

    {
		addOp(OP_SOUNDID);
	;}
    break;

  case 403:

    {
		addOp(OP_SOUNDPOSITION);
	;}
    break;

  case 404:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPOSITION);
	;}
    break;

  case 405:

    {
		addOp(OP_SOUNDSTATE);
	;}
    break;

  case 406:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDSTATE);
	;}
    break;

  case 407:

    {
		addOp(OP_SOUNDLENGTH);
	;}
    break;

  case 408:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDLENGTH);
	;}
    break;

  case 409:

    {
		addOp(OP_SOUNDSAMPLERATE);
	;}
    break;

  case 410:

    {
		addOp(OP_IMAGEWIDTH);
	;}
    break;

  case 411:

    {
		addOp(OP_IMAGEHEIGHT);
	;}
    break;

  case 412:

    {
		addOp(OP_IMAGEPIXEL);
	;}
    break;

  case 413:

    {
		addIntOp(OP_PUSHINT,0);		// default decimal places
		addOp(OP_ROUND);
	;}
    break;

  case 414:

    {
		addOp(OP_ROUND);
	;}
    break;

  case 415:

    {
		addIntOp(OP_VAR_ASSIGNED, varnumber[--nvarnumber]);
	;}
    break;

  case 416:

    {
		addIntOp(OP_ARR_ASSIGNED, varnumber[--nvarnumber]);
	;}
    break;

  case 417:

    {
		addIntOp(OP_ALEN, varnumber[--nvarnumber]);
	;}
    break;

  case 418:

    {
		addIntOp(OP_ALENROWS, varnumber[--nvarnumber]);
	;}
    break;

  case 419:

    {
		addIntOp(OP_ALENCOLS, varnumber[--nvarnumber]);
	;}
    break;

  case 420:

    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 421:

    { addOp(OP_CHR); ;}
    break;

  case 422:

    { addOp(OP_STRING); ;}
    break;

  case 423:

    { addOp(OP_UPPER); ;}
    break;

  case 424:

    { addOp(OP_LOWER); ;}
    break;

  case 425:

    { addOp(OP_MID); ;}
    break;

  case 426:

    { addIntOp(OP_PUSHINT, 1); addOp(OP_MIDX); ;}
    break;

  case 427:

    { addOp(OP_MIDX); ;}
    break;

  case 428:

    { addOp(OP_LEFT); ;}
    break;

  case 429:

    { addOp(OP_RIGHT); ;}
    break;

  case 430:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 431:

    { addOp(OP_READ); ;}
    break;

  case 432:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 433:

    { addOp(OP_READLINE); ;}
    break;

  case 434:

    { addOp(OP_CURRENTDIR); ;}
    break;

  case 435:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBSTRING); ;}
    break;

  case 436:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBSTRING); ;}
    break;

  case 437:

    {
		addOp(OP_DBSTRING); ;}
    break;

  case 438:

    { addOp(OP_LASTERRORMESSAGE); ;}
    break;

  case 439:

    { addOp(OP_LASTERROREXTRA); ;}
    break;

  case 440:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETREAD); ;}
    break;

  case 441:

    { addOp(OP_NETREAD); ;}
    break;

  case 442:

    { addOp(OP_NETADDRESS); ;}
    break;

  case 443:

    { addOp(OP_MD5); ;}
    break;

  case 444:

    { addOp(OP_GETSETTING); ;}
    break;

  case 445:

    { addOp(OP_DIR); ;}
    break;

  case 446:

    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_DIR); ;}
    break;

  case 447:

    {
		addIntOp(OP_PUSHINT, 0);	// case sens flag
		addOp(OP_REPLACE);
	;}
    break;

  case 448:

    { addOp(OP_REPLACE); ;}
    break;

  case 449:

    { addOp(OP_REPLACEX); ;}
    break;

  case 450:

    {
		addOp(OP_SERIALIZE);
	;}
    break;

  case 451:

    {
		addStringOp(OP_PUSHSTRING, ""); // no delimiter
		addOp(OP_STACKDUP);
		addOp(OP_IMPLODE);
	;}
    break;

  case 452:

    {
		addOp(OP_STACKDUP);				// same delimiter for rows and columns
		addOp(OP_IMPLODE);
	;}
    break;

  case 453:

    {
		addOp(OP_IMPLODE);
	;}
    break;

  case 454:

    {
		addStringOp(OP_PUSHSTRING, "");
		addOp(OP_PROMPT); ;}
    break;

  case 455:

    {
		addOp(OP_PROMPT); ;}
    break;

  case 456:

    {
		addIntOp(OP_PUSHINT,2);	// radix
		addOp(OP_TORADIX);
	;}
    break;

  case 457:

    {
		addIntOp(OP_PUSHINT,16);	// radix
		addOp(OP_TORADIX);
	;}
    break;

  case 458:

    {
		addIntOp(OP_PUSHINT,8);	// radix
		addOp(OP_TORADIX);
	;}
    break;

  case 459:

    {
		addOp(OP_TORADIX);
	;}
    break;

  case 460:

    {
		addOp(OP_DEBUGINFO);
	;}
    break;

  case 461:

    { addOp(OP_ISNUMERIC); ;}
    break;

  case 462:

    { addOp(OP_LTRIM); ;}
    break;

  case 463:

    { addOp(OP_RTRIM); ;}
    break;

  case 464:

    { addOp(OP_TRIM); ;}
    break;

  case 465:

    { addStringOp(OP_PUSHSTRING, IMAGETYPE_BMP); ;}
    break;

  case 466:

    { addStringOp(OP_PUSHSTRING, IMAGETYPE_JPG); ;}
    break;

  case 467:

    { addStringOp(OP_PUSHSTRING, IMAGETYPE_PNG); ;}
    break;

  case 468:

    {
		addOp(OP_SOUNDLOAD);
	;}
    break;

  case 469:

    {
		addIntOp(OP_PUSHINT, 2);	// 2 columns
		addIntOp(OP_PUSHINT, 1);	// 1 row
		addOp(OP_LIST2ARRAY);
		addOp(OP_SOUNDLOAD);
	;}
    break;

  case 470:

    {
		addOp(OP_SOUNDLOADRAW);
	;}
    break;

  case 471:

    {
		addOp(OP_IMAGENEW);
	;}
    break;

  case 472:

    {
		addIntOp(OP_PUSHINT, 0x00);
		addOp(OP_IMAGENEW);
	;}
    break;

  case 473:

    {
		addOp(OP_IMAGELOAD);
	;}
    break;

  case 474:

    {
		addIntOp(OP_PUSHINT, 5); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 475:

    {
		addIntOp(OP_PUSHINT, 4); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 476:

    {
		addIntOp(OP_PUSHINT, 1); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 477:

    {
		addIntOp(OP_PUSHINT, 0); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 478:

    {
		addStringOp(OP_PUSHSTRING, " ");
		addOp(OP_LJUST);
	;}
    break;

  case 479:

    {
		addOp(OP_LJUST);
	;}
    break;

  case 480:

    {
		addStringOp(OP_PUSHSTRING, " ");
		addOp(OP_RJUST);
	;}
    break;

  case 481:

    {
		addOp(OP_RJUST);
	;}
    break;

  case 482:

    {
		addStringOp(OP_PUSHSTRING, "0");
		addOp(OP_RJUST);
	;}
    break;

  case 483:

    {
		addIntOp(OP_VAR_GET, varnumber[--nvarnumber]);
	;}
    break;

  case 484:

    {
		addOp(OP_LIST2ARRAY);
	;}
    break;

  case 485:

    {
		addOp(OP_LIST2MAP);
	;}
    break;

  case 486:

    {
		addOp(OP_UNSERIALIZE);
	;}
    break;

  case 487:

    {
		addIntOp(OP_PUSHINT, 0);	// case sensitive flag
		addOp(OP_EXPLODE);
	;}
    break;

  case 488:

    {
		addOp(OP_EXPLODE);
	;}
    break;

  case 489:

    {
		addOp(OP_EXPLODEX);
	;}
    break;

  case 490:

    {
		addIntOp(OP_PUSHINT, SLICE_ALL);	// get everything
		addOp(OP_GETSLICE);
	;}
    break;

  case 491:

    {
		addOp(OP_GETSLICE);
	;}
    break;

  case 633:

    {
		// start a case block
		newIf(linenumber, IFTABLETYPEBEGINCASE,-1);
	;}
    break;

  case 634:

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
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
				//
				numifs--;
			}
		}
		//
	;}
    break;

  case 635:

    {
		//
		// add branch to the end if false
		addIntOp(OP_BRANCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
		//
		// put new CASE on the frame for the IF
		newIf(linenumber, IFTABLETYPECASE,-1);
	;}
    break;

  case 636:

    {
	//
	// create jump around from end of the TRY to end of the CATCH
	// OP_OFFERRORCATCH label - close try/catch trap and jump over the CATCH part
	addIntOp(OP_OFFERRORCATCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
	//
	if (numifs>0) {
		if (iftabletype[numifs-1]==IFTABLETYPETRY) {
			//
			// resolve the try onerrorcatch to the catch address
			symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
			numifs--;
			//
			// put new if on the frame for the catch
			newIf(linenumber, IFTABLETYPECATCH,-1);
		} else {
			errorcode = testIfOnTableError(numincludes);
			linenumber = testIfOnTable(numincludes);
			return -1;
		}
	} else {
		errorcode = COMPERR_CATCH;
		return -1;
	}
;}
    break;

  case 637:

    {
		//
		// create internal symbol and add to the label table for the top of the loop
		symtableaddress[getInternalSymbol(nextifid,INTERNALSYMBOLTOP)] = wordOffset;
		//
		// add to if frame
		newIf(linenumber, IFTABLETYPEDO, -1);
	;}
    break;

  case 638:

    {
		//
		// create jump around from end of the THEN to end of the ELSE
		addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
		//
		if (numifs>0) {
			if (iftabletype[numifs-1]==IFTABLETYPEIF) {
				//
				// resolve the label on the if to the current location
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
				numifs--;
				//
				// put new if on the frame for the else
				newIf(linenumber, IFTABLETYPEELSE, -1);
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
					symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
					//
					numifs--;
					// put new if on the frame for the else
					newIf(linenumber, IFTABLETYPEELSE, -1);
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
	;}
    break;

  case 639:

    {
				// add label for last case branchfalse to jump to
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPECASE || iftabletype[numifs-1]==IFTABLETYPEELSE) {
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
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
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
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
			;}
    break;

  case 640:

    {
				// if there is an if branch or jump on the iftable stack get where it is
				// in the wordcode array and then put the current wordcode address there
				// - so we can jump over code
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEIF||iftabletype[numifs-1]==IFTABLETYPEELSE) {
						//
						// resolve the label on the if/else to the current location
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
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
			;}
    break;

  case 641:

    {
				// if there is an if branch or jump on the iftable stack get where it is
				// in the wordcode array and then put the current wordcode address there
				// - so we can jump over code
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPECATCH) {
						//
						// resolve the label on the Catch to the current location
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
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
			;}
    break;

  case 642:

    {
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEWHILE) {
						//
						// jump to the top
						addIntOp(OP_GOTO, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE));
						//
						// resolve the label to the bottom
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
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
			;}
    break;

  case 643:

    {
					//
					// add branch to the end if false
					addIntOp(OP_BRANCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
					//
					// put new if on the frame for the IF
					newIf(linenumber, IFTABLETYPEIF, -1);
			;}
    break;

  case 644:

    {
				// if there is an if branch or jump on the iftable stack get where it is
				// in the wordcode array and then resolve the lable
				if (numifs>0) {
					symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
					numifs--;
				}
			;}
    break;

  case 645:

    {
				//
				// resolve the label on the else to the current location
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
				numifs--;
			;}
    break;

  case 646:

    {
				//
				// create jump around from end of the THEN to end of the ELSE
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// jump point for else
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
				numifs--;
				//
				// put new if on the frame for the else
				newIf(linenumber, IFTABLETYPEELSE, -1);
			;}
    break;

  case 647:

    {
				//
				// add on error branch
                                addIntOp(OP_ONERRORCATCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// put new if on the frame for the TRY
				newIf(linenumber, IFTABLETYPETRY, -1);
			;}
    break;

  case 648:

    {
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEDO) {
						//
						// create label for CONTINUE DO
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE)] = wordOffset;
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
			;}
    break;

  case 649:

    {
				//
				// branch back to top if condition holds
				addIntOp(OP_BRANCH, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLTOP));
				//
				// create label for EXIT DO
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
				numifs--;
			;}
    break;

  case 650:

    {
				//
				// create internal symbol and add to the label table for the top of the loop
				newIf(linenumber, IFTABLETYPEWHILE, -1);
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE)] = wordOffset;
			;}
    break;

  case 651:

    {
				//
				// add branch to end if false
				addIntOp(OP_BRANCH, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT));
			;}
    break;

  case 658:

    {
				for(int t=numargs-1;t>=0;t--) {
					addIntOp(OP_MAP_DIM, args[t]);
				}
				numargs=0;	// clear the list for next function
			;}
    break;

  case 659:

    {
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
			;}
    break;

  case 660:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 661:

    {
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
			;}
    break;

  case 662:

    {
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 663:

    {
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
			;}
    break;

  case 664:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 665:

    {
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 666:

    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_ARRAYFILL, varnumber[--nvarnumber]);
			;}
    break;

  case 667:

    {
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
			;}
    break;

  case 668:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 0);		// just fill unassigned
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 669:

    {
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
			;}
    break;

  case 670:

    {
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 0);		// just fill unassigned
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 671:

    {
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
			;}
    break;

  case 672:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 0);		// just fill unassigned
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 673:

    {
				addOp(OP_PAUSE);
			;}
    break;

  case 674:

    {
				addOp(OP_THROWERROR);
			;}
    break;

  case 675:

    {
				addOp(OP_CLS);
			;}
    break;

  case 676:

    {
				// push the color clear if there are no arguments
				addIntOp(OP_PUSHINT, 0x00);
				//addIntOp(OP_PUSHINT, 0x00);
				//addIntOp(OP_PUSHINT, 0x00);
				//addIntOp(OP_PUSHINT, 0x00);
				//addIntOp(OP_PUSHINT, 0x00);
				//addOp(OP_RGB);
				addOp(OP_CLG);
			;}
    break;

  case 677:

    {
				addOp(OP_CLG);
			;}
    break;

  case 678:

    {
				addOp(OP_FASTGRAPHICS);
			;}
    break;

  case 679:

    {
                                addFloatOp(OP_PUSHFLOAT, 1.0);
				addOp(OP_GRAPHSIZE);
			;}
    break;

  case 680:

    {
					addOp(OP_GRAPHSIZE);
			;}
    break;

  case 681:

    {
				addOp(OP_REFRESH);
			;}
    break;

  case 682:

    {
				addOp(OP_END);
			;}
    break;

  case 683:

    {
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 684:

    {
				// a[r,c]++ - NO PUSH as this is a statement
				int v = varnumber[--nvarnumber];
				addOp(OP_STACKDUP2);			// save indexes
				addIntOp(OP_ARR_GET, v);		// get current value
				addIntOp(OP_PUSHINT,1);			// add 1
				addOp(OP_ADD);
				addIntOp(OP_ARR_SET, v);		// assign new value
			;}
    break;

  case 685:

    {
				// a[r,c]-- - NO PUSH as this is a statement
				int v = varnumber[--nvarnumber];
				addOp(OP_STACKDUP2);			// save indexes
				addIntOp(OP_ARR_GET, v);		// get current value
				addIntOp(OP_PUSHINT,-1);		// subtract 1
				addOp(OP_ADD);
				addIntOp(OP_ARR_SET, v);		// assign new value
			;}
    break;

  case 686:

    {
				// a[b,c] += n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_ARR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_ADD);
				addIntOp(OP_ARR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 687:

    {
				// a[b,c] -= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_ARR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_STACKSWAP);
				addOp(OP_SUB);
				addIntOp(OP_ARR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 688:

    {
				// a[b,c] *= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_ARR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_MUL);
				addIntOp(OP_ARR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 689:

    {
				// a[b,c] /= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_ARR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_STACKSWAP);
				addOp(OP_DIV);
				addIntOp(OP_ARR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 690:

    {
				// a[b,c] &= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_ARR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_STACKSWAP);
				addOp(OP_BINARYAND);
				addIntOp(OP_ARR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 691:

    {
				// a[b,c] /= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_ARR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_STACKSWAP);
				addOp(OP_CONCATENATE);
				addIntOp(OP_ARR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 692:

    {
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[--nvarnumber]);
			;}
    break;

  case 693:

    {
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 694:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
			;}
    break;

  case 695:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT,-1);
				addOp(OP_ADD);
				addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
			;}
    break;

  case 696:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_ADD);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 697:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_SUB);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 698:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_MUL);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 699:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_DIV);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 700:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_BINARYAND);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 701:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_CONCATENATE);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 702:

    {
				// add to iftable to make sure it is not broken with an if
				// do, while, else, and to report if it is
				// next ed before end of program
				int var =  varnumber[--nvarnumber];
				newIf(linenumber, IFTABLETYPEFOR, var);
				// push default step 1 and exit address
				addIntOp(OP_PUSHINT, 1); //step
				addIntOp(OP_PUSHLABEL, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT));
				addIntOp(OP_FOR, var);
			;}
    break;

  case 703:

    {
				// add to iftable to make sure it is not broken with an if
				// do, while, else, and to report if it is
				// next ed before end of program
				int var =  varnumber[--nvarnumber];
				newIf(linenumber, IFTABLETYPEFOR, var);
				// push exit address
				addIntOp(OP_PUSHLABEL, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT));
				addIntOp(OP_FOR, var);
			;}
    break;

  case 704:

    {
				// add to iftable to make sure it is not broken with an if
				// do, while, else, and to report if it is
				// next ed before end of program
				int var =  varnumber[--nvarnumber];
				newIf(linenumber, IFTABLETYPEFOR, var);
				addIntOp(OP_PUSHLABEL, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT));
				addIntIntOp(OP_FOREACH, var, -1 );
			;}
    break;

  case 705:

    {
				// add to iftable to make sure it is not broken with an if
				// do, while, else, and to report if it is
				// next ed before end of program
				int value_var =  varnumber[--nvarnumber];
				int key_var =  varnumber[--nvarnumber];
				newIf(linenumber, IFTABLETYPEFOR, key_var);
				addIntOp(OP_PUSHLABEL, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT));
				addIntIntOp(OP_FOREACH, key_var, value_var);
			;}
    break;

  case 706:

    {
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEFOR) {
						if (iftablevariable[numifs-1]==varnumber[nvarnumber-1]) {
							symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE)] = wordOffset;
							addOp(OP_NEXT);
							symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
							numifs--;
						} else {
							errorcode = COMPERR_NEXTWRONGFOR;
							return -1;
						}
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_NEXT;
					return -1;
				}
			;}
    break;

  case 707:

    {
				// allow relaxed next without the variable name
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEFOR) {
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE)] = wordOffset;
						addOp(OP_NEXT);
						symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
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
			;}
    break;

  case 708:

    {
				if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
					errorcode = COMPERR_FUNCTIONGOTO;
					return -1;
				}
				addIntOp(OP_GOTO, varnumber[--nvarnumber]);
			;}
    break;

  case 709:

    {
				if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
					errorcode = COMPERR_FUNCTIONGOTO;
					return -1;
				}
				addIntOp(OP_GOSUB, varnumber[--nvarnumber]);
				addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
			;}
    break;

  case 710:

    {
				addIntOp(OP_PUSHINT, 0); //push number of arguments passed to compare with SUBROUTINE definition
				addIntOp(OP_CALLSUBROUTINE, varnumber[--nvarnumber]);
				addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
			;}
    break;

  case 711:

    {
					addIntOp(OP_PUSHINT, listlen); //push number of arguments passed to compare with SUBROUTINE definition
					addIntOp(OP_CALLSUBROUTINE, varnumber[--nvarnumber]);
					addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
			;}
    break;

  case 712:

    {
				addOp(OP_OFFERROR);
			;}
    break;

  case 713:

    {
				addIntOp(OP_ONERRORGOSUB, varnumber[--nvarnumber]);
			;}
    break;

  case 714:

    {
				addIntOp(OP_ONERRORCALL, varnumber[--nvarnumber]);
			;}
    break;

  case 715:

    {
				errorcode = COMPERR_ONERRORCALL;
				return -1;
			;}
    break;

  case 716:

    {
				if (functionDefSymbol!=-1) {
					// if we are defining a function return pushes a variable value
					addIntOp(OP_VAR_GET, functionDefSymbol);
					addOp(OP_DECREASERECURSE);
				}
				if (subroutineDefSymbol!=-1) {
					// if we are defining a subroutine
					addOp(OP_DECREASERECURSE);
				}
				addOp(OP_RETURN);
			;}
    break;

  case 717:

    {
				if (functionDefSymbol!=-1) {
					// value on stack gets returned
					addOp(OP_DECREASERECURSE);
					addOp(OP_RETURN);
				} else {
					errorcode = COMPERR_RETURNVALUE;
					return -1;
				}
			;}
    break;

  case 718:

    {
				addOp(OP_STACKDUP);
				addOp(OP_SETCOLOR);
			;}
    break;

  case 719:

    {
				addOp(OP_SETCOLOR);
			;}
    break;

  case 720:

    {
				addIntOp(OP_PUSHINT, 255);
				addOp(OP_RGB);
				addOp(OP_STACKDUP);
				addOp(OP_SETCOLOR);
				newParseWarning(COMPWARNING_DEPRECATED_FORM);
			;}
    break;

  case 721:

    {
				addIntOp(OP_PUSHINT, 2);	// 2 columns (this)
				addIntOp(OP_PUSHINT, 1);	// 1 row
				addIntOp(OP_PUSHINT, 2);	// 2 columns (max)
				addOp(OP_LIST2ARRAY);
				addOp(OP_SOUND);
			;}
    break;

  case 722:

    {
				addOp(OP_SOUND);
			;}
    break;

  case 723:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPLAY);
	;}
    break;

  case 724:

    {
		addIntOp(OP_PUSHINT, 2);	// 2 columns
		addIntOp(OP_PUSHINT, 1);	// 1 row
		addOp(OP_LIST2ARRAY);
		addOp(OP_SOUNDPLAY);
	;}
    break;

  case 725:

    {
		// either string - lodresource
		// int - player number
		// array - from a variable
		addOp(OP_SOUNDPLAY);
	;}
    break;

  case 726:

    {
		addOp(OP_SOUNDPAUSE);
	;}
    break;

  case 727:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPAUSE);
	;}
    break;

  case 728:

    {
		addOp(OP_SOUNDPLAYEROFF);
	;}
    break;

  case 729:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPLAYEROFF);
	;}
    break;

  case 730:

    {
		addOp(OP_SOUNDSTOP);
	;}
    break;

  case 731:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDSTOP);
	;}
    break;

  case 732:

    {
		addOp(OP_SOUNDWAIT);
	;}
    break;

  case 733:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDWAIT);
	;}
    break;

  case 734:

    {
		addIntOp(OP_PUSHINT,0);
		addOp(OP_SOUNDWAVEFORM);
	;}
    break;

  case 735:

    {
		addOp(OP_SOUNDWAVEFORM);
	;}
    break;

  case 736:

    {
		addOp(OP_SOUNDSYSTEM);
	;}
    break;

  case 737:

    {
		addOp(OP_SOUNDNOENVELOPE);
	;}
    break;

  case 738:

    {
		addOp(OP_SOUNDENVELOPE);
	;}
    break;

  case 739:

    {
		addOp(OP_SOUNDENVELOPE);
	;}
    break;

  case 740:

    {
		addOp(OP_SOUNDNOHARMONICS);
	;}
    break;

  case 741:

    {
		addOp(OP_SOUNDHARMONICS);
	;}
    break;

  case 742:

    {
		addOp(OP_SOUNDHARMONICS);
	;}
    break;

  case 743:

    {
		addOp(OP_SOUNDFADE);
	;}
    break;

  case 744:

    {
		addOp(OP_STACKTOPTO2);
		addOp(OP_STACKTOPTO2);
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_STACKSWAP2);
		addOp(OP_SOUNDFADE);
	;}
    break;

  case 745:

    {
		addOp(OP_SOUNDSEEK);
	;}
    break;

  case 746:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_SOUNDSEEK);
	;}
    break;

  case 747:

    {
		addOp(OP_SOUNDVOLUME);
	;}
    break;

  case 748:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_SOUNDVOLUME);
	;}
    break;

  case 749:

    {
		addOp(OP_SOUNDLOOP);
	;}
    break;

  case 750:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_SOUNDLOOP);
	;}
    break;

  case 751:

    {
				addOp(OP_PLOT);
			;}
    break;

  case 752:

    {
				addOp(OP_LINE);
			;}
    break;

  case 753:

    {
		addOp(OP_CIRCLE);
	;}
    break;

  case 754:

    {
		addOp(OP_ELLIPSE);
	;}
    break;

  case 755:

    {
		addIntOp(OP_PUSHINT, 5); // with bounding circle
		addOp(OP_ARC);
	;}
    break;

  case 756:

    {
		addIntOp(OP_PUSHINT, 6); // with bounding rectangle
		addOp(OP_ARC);
	;}
    break;

  case 757:

    {
		addIntOp(OP_PUSHINT, 5); // with bounding circle
		addOp(OP_CHORD);
	;}
    break;

  case 758:

    {
		addIntOp(OP_PUSHINT, 6); // with bounding rectangle
		addOp(OP_CHORD);
	;}
    break;

  case 759:

    {
		addIntOp(OP_PUSHINT, 5); // with bounding circle
		addOp(OP_PIE);
	;}
    break;

  case 760:

    {
		addIntOp(OP_PUSHINT, 6); // with bounding rectangle
		addOp(OP_PIE);
	;}
    break;

  case 761:

    {
		addOp(OP_RECT);
	;}
    break;

  case 762:

    {
		addOp(OP_STACKDUP);
		addOp(OP_ROUNDEDRECT);
	;}
    break;

  case 763:

    {
		addOp(OP_ROUNDEDRECT);
	;}
    break;

  case 764:

    {
		addOp(OP_TEXT);
	;}
    break;

  case 765:

    {
		addIntOp(OP_PUSHINT, 0); // flags
		addOp(OP_TEXTBOX);
	;}
    break;

  case 766:

    {
		addOp(OP_TEXTBOX);
	;}
    break;

  case 767:

    {
		addOp(OP_FONT);
	;}
    break;

  case 768:

    {
		addIntOp(OP_PUSHINT, 0); // font is not italic
		addOp(OP_FONT);
	;}
    break;

  case 769:

    {
		addIntOp(OP_PUSHINT, -1); // default weight
		addIntOp(OP_PUSHINT, 0); // font is not italic
		addOp(OP_FONT);
	;}
    break;

  case 770:

    {
		addIntOp(OP_PUSHINT, -1); // default size
		addIntOp(OP_PUSHINT, -1); // default weight
		addIntOp(OP_PUSHINT, 0); // font is not italic
		addOp(OP_FONT);
	;}
    break;

  case 771:

    {
				addOp(OP_SAY);
			;}
    break;

  case 772:

    {
				addOp(OP_SYSTEM);
			;}
    break;

  case 773:

    {
				addOp(OP_VOLUME);
			;}
    break;

  case 774:

    {
				addOp(OP_POLY);
			;}
    break;

  case 775:

    {
				addOp(OP_STAMP);
			;}
    break;

  case 776:

    {
				addFloatOp(OP_PUSHFLOAT, 0.0); // default rotate
				addOp(OP_STACKSWAP);
				addOp(OP_STAMP);
			;}
    break;

  case 777:

    {
				addFloatOp(OP_PUSHFLOAT, 1.0); // default scale
				addOp(OP_STACKSWAP);
				addFloatOp(OP_PUSHFLOAT, 0.0); // default rotate
				addOp(OP_STACKSWAP);
				addOp(OP_STAMP);
			;}
    break;

  case 778:

    {
				addIntOp(OP_PUSHINT, 0); // file number zero
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 0); // not binary
				addOp(OP_OPEN);
			;}
    break;

  case 779:

    {
				addIntOp(OP_PUSHINT, 0); // not binary
				addOp(OP_OPEN);
			;}
    break;

  case 780:

    {
				addIntOp(OP_PUSHINT, 0); // file number zero
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 1); // binary
				addOp(OP_OPEN);
			;}
    break;

  case 781:

    {
				addIntOp(OP_PUSHINT, 1); // binary
				addOp(OP_OPEN);
			;}
    break;

  case 782:

    {
				addIntOp(OP_PUSHINT, 9600); // baud
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 783:

    {
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 784:

    {
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 785:

    {
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 786:

    {
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 787:

    {
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 788:

    {
				addIntOp(OP_PUSHINT, 0);  // file number zero
				addOp(OP_STACKSWAP);
				addOp(OP_WRITE);
			;}
    break;

  case 789:

    {
				addOp(OP_WRITE);
			;}
    break;

  case 790:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_WRITELINE);
			;}
    break;

  case 791:

    {
				addOp(OP_WRITELINE);
			;}
    break;

  case 792:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_WRITEBYTE);
			;}
    break;

  case 793:

    {
				addOp(OP_WRITEBYTE);
			;}
    break;

  case 794:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_CLOSE);
			;}
    break;

  case 795:

    {
				addOp(OP_CLOSE);
			;}
    break;

  case 796:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_RESET);
			;}
    break;

  case 797:

    {
				addOp(OP_RESET);
			;}
    break;

  case 798:

    {
				addOp(OP_SEED);
			;}
    break;

  case 799:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_SEEK);
			;}
    break;

  case 800:

    {
				addOp(OP_SEEK);
			;}
    break;

  case 801:

    {
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 802:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 803:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 804:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 805:

    {
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 806:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 807:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 808:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 809:

    {
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 810:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 811:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 812:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 813:

    {
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 814:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 815:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 816:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 817:

    {
				addIntOp(OP_PUSHINT, 0); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 818:

    {
				addIntOp(OP_PUSHINT, 1); //push number of arguments passed
				addIntOp(OP_PUSHINT, 0); // suppress NL
				addOp(OP_PRINT);
			;}
    break;

  case 819:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 820:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 821:

    {
				addIntOp(OP_PUSHINT, 0); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 822:

    {
				addIntOp(OP_PUSHINT, 1); //push number of arguments passed
				addIntOp(OP_PUSHINT, 0); // suppress NL
				addOp(OP_PRINT);
			;}
    break;

  case 823:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 824:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 825:

    {
				addOp(OP_WAVPAUSE);
			;}
    break;

  case 826:

    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_WAVPLAY);
			;}
    break;

  case 827:

    {
				addOp(OP_WAVPLAY);
			;}
    break;

  case 828:

    {
				addOp(OP_WAVSEEK);
			;}
    break;

  case 829:

    {
				addOp(OP_WAVSTOP);
			;}
    break;

  case 830:

    {
				addOp(OP_WAVWAIT);
			;}
    break;

  case 831:

    {
				addOp(OP_PUTSLICE);
			;}
    break;

  case 832:

    {
				addIntOp(OP_PUSHINT, 1); // scale
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			;}
    break;

  case 833:

    {
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			;}
    break;

  case 834:

    {
				addOp(OP_IMGLOAD);
			;}
    break;

  case 835:

    {
				addOp(OP_SPRITEDIM);
			;}
    break;

  case 836:

    {
				addOp(OP_SPRITELOAD);
			;}
    break;

  case 837:

    {
				addOp(OP_SPRITESLICE);
			;}
    break;

  case 838:

    {
				addOp(OP_SPRITEPOLY);
			;}
    break;

  case 839:

    {
				addIntOp(OP_PUSHINT,3);	// nr of arguments
				addOp(OP_SPRITEPLACE);
			;}
    break;

  case 840:

    {
				addIntOp(OP_PUSHINT,4);	// nr of arguments
				addOp(OP_SPRITEPLACE);
			;}
    break;

  case 841:

    {
				addIntOp(OP_PUSHINT,5);	// nr of arguments
				addOp(OP_SPRITEPLACE);
			;}
    break;

  case 842:

    {
					addIntOp(OP_PUSHINT,6);	// nr of arguments
					addOp(OP_SPRITEPLACE);
			;}
    break;

  case 843:

    {
				addIntOp(OP_PUSHINT,3);	// nr of arguments
				addOp(OP_SPRITEMOVE);
			;}
    break;

  case 844:

    {
				addIntOp(OP_PUSHINT,4);	// nr of arguments
				addOp(OP_SPRITEMOVE);
			;}
    break;

  case 845:

    {
				addIntOp(OP_PUSHINT,5);	// nr of arguments
				addOp(OP_SPRITEMOVE);
			;}
    break;

  case 846:

    {
					addIntOp(OP_PUSHINT,6);	// nr of arguments
					addOp(OP_SPRITEMOVE);
			;}
    break;

  case 847:

    {
				addOp(OP_SPRITEHIDE);
			;}
    break;

  case 848:

    {
				addOp(OP_SPRITESHOW);
			;}
    break;

  case 849:

    {
				addOp(OP_CLICKCLEAR);
			;}
    break;

  case 850:

    {
				addOp(OP_CHANGEDIR);
			;}
    break;

  case 851:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPEN);
			;}
    break;

  case 852:

    {
				addOp(OP_DBOPEN);
			;}
    break;

  case 853:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_DBCLOSE);
			;}
    break;

  case 854:

    {
				addOp(OP_DBCLOSE);
			;}
    break;

  case 855:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addOp(OP_DBEXECUTE);
			;}
    break;

  case 856:

    {
				addOp(OP_DBEXECUTE);
			;}
    break;

  case 857:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPENSET);
			;}
    break;

  case 858:

    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPENSET);
			;}
    break;

  case 859:

    {
				addOp(OP_DBOPENSET);
			;}
    break;

  case 860:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBCLOSESET);
			;}
    break;

  case 861:

    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBCLOSESET);
			;}
    break;

  case 862:

    {
				addOp(OP_DBCLOSESET);
			;}
    break;

  case 863:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_NETLISTEN);
			;}
    break;

  case 864:

    {
				addOp(OP_NETLISTEN);
			;}
    break;

  case 865:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKTOPTO2);
				addOp(OP_NETCONNECT);
			;}
    break;

  case 866:

    {
				addOp(OP_NETCONNECT);
			;}
    break;

  case 867:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_NETWRITE);
			;}
    break;

  case 868:

    {
				addOp(OP_NETWRITE);
			;}
    break;

  case 869:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_NETCLOSE);
			;}
    break;

  case 870:

    {
				addOp(OP_NETCLOSE);
			;}
    break;

  case 871:

    {
				addOp(OP_KILL);
			;}
    break;

  case 872:

    {
				addOp(OP_SETSETTING);
			;}
    break;

  case 873:

    {
				addOp(OP_PORTOUT);
			;}
    break;

  case 874:

    {
				addStringOp(OP_PUSHSTRING, "PNG");
				addOp(OP_IMGSAVE);
			;}
    break;

  case 875:

    {
				addOp(OP_IMGSAVE);
			;}
    break;

  case 876:

    {
				addOp(OP_EDITVISIBLE);
			;}
    break;

  case 877:

    {
				addOp(OP_GRAPHVISIBLE);
			;}
    break;

  case 878:

    {
				addOp(OP_OUTPUTVISIBLE);
			;}
    break;

  case 879:

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
			;}
    break;

  case 880:

    {
				addOp(OP_PENWIDTH);
			;}
    break;

  case 881:

    {
				addOp(OP_ALERT);
			;}
    break;

  case 882:

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
			;}
    break;

  case 883:

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
			;}
    break;

  case 884:

    {
				// find most recent WHILE and jump to CONTINUE
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEWHILE) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_GOTO, getInternalSymbol(iftableid[n],INTERNALSYMBOLCONTINUE));
				} else {
                                        errorcode = COMPERR_CONTINUEWHILE;
					return -1;
				}
			;}
    break;

  case 885:

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
			;}
    break;

  case 886:

    {
				// find most recent FOR and jump to exit
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEFOR) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_EXITFOR, getInternalSymbol(iftableid[n],INTERNALSYMBOLEXIT));
				} else {
					errorcode = COMPERR_EXITFOR;
					return -1;
				}
			;}
    break;

  case 887:

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
			;}
    break;

  case 888:

    {
				addOp(OP_PRINTERCANCEL);
			;}
    break;

  case 889:

    {
				addOp(OP_PRINTEROFF);
			;}
    break;

  case 890:

    {
				addOp(OP_PRINTERON);
			;}
    break;

  case 891:

    {
				addOp(OP_PRINTERPAGE);
			;}
    break;

  case 892:

    {
				if (numifs>0) {
					errorcode = COMPERR_FUNCTIONNOTHERE;
					return -1;
				}
				//
				// $2 is the symbol for the function - add the start to the label table
				functionDefSymbol = varnumber[--nvarnumber];
				//
				//check if name of function is already used by a label, subroutine or another function
				if (symtableaddress[functionDefSymbol] != -1) {
						errorcode = COMPERR_LABELREDEFINED;
						return -1;
				}
				//
				// create jump around function definition (use nextifid and 0 for jump after)
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// create the new if frame for this function
				symtableaddress[functionDefSymbol] = wordOffset;
				symtableaddresstype[functionDefSymbol] = ADDRESSTYPE_FUNCTION;
				newIf(linenumber, IFTABLETYPEFUNCTION, -1);
				//
				// store the number of the arguments required by FUNCTION
				// to check if number of arguments passed match definition when is called
				symtableaddressargs[functionDefSymbol] = numargs;
				//
				// add the assigns of the function arguments
				addOp(OP_INCREASERECURSE);
				{ 	int t;
					for(t=numargs-1;t>=0;t--) {
						addIntOp(OP_VAR_SET, args[t]);
					}
				}
				//
				// initialize return variable
				addIntOp(OP_PUSHINT, 0);
				addIntOp(OP_VAR_SET, functionDefSymbol);
				//
				numargs=0;	// clear the list for next function
			;}
    break;

  case 893:

    {
				if (numifs>0) {
					errorcode = COMPERR_FUNCTIONNOTHERE;
					return -1;
				}
				//
				// $2 is the symbol for the subroutine - add the start to the label table
				subroutineDefSymbol = varnumber[--nvarnumber];
				//
				//check if name of subroutine is already used by a label, function or another subroutine
				if (symtableaddress[subroutineDefSymbol] != -1) {
						errorcode = COMPERR_LABELREDEFINED;
						return -1;
				}
				//
				// create jump around subroutine definition (use nextifid and 0 for jump after)
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// create the new if frame for this subroutine
				symtableaddress[subroutineDefSymbol] = wordOffset;
				symtableaddresstype[subroutineDefSymbol] = ADDRESSTYPE_SUBROUTINE;
				newIf(linenumber, IFTABLETYPESUBROUTINE, -1);
				//
				// store the number of the arguments required by SUBROUTINE
				// to check if number of arguments passed match definition when is called
				symtableaddressargs[subroutineDefSymbol] = numargs;
				//
				// add the assigns of the function arguments
				addOp(OP_INCREASERECURSE);
				{ 	int t;
					for(t=numargs-1;t>=0;t--) {
						addIntOp(OP_VAR_SET, args[t]);
					}
				}
				numargs=0;	// clear the list for next function
			;}
    break;

  case 894:

    {
				if (numifs>0) {
				if (iftabletype[numifs-1]==IFTABLETYPEFUNCTION) {
					//
					// add return if there is not one
					addIntOp(OP_VAR_GET, functionDefSymbol);
					addOp(OP_DECREASERECURSE);
					addOp(OP_RETURN);
					//
					// add address for jump around function definition
					symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
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
		;}
    break;

  case 895:

    {
			if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPESUBROUTINE) {
					addOp(OP_DECREASERECURSE);
					addOp(OP_RETURN);
					//
					// add address for jump around function definition
					symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
					subroutineDefSymbol = -1;
					//
					numifs--;
				} else {
					errorcode = testIfOnTableError(numincludes);
					linenumber = testIfOnTable(numincludes);
					return -1;
				}
			} else {
                                errorcode = COMPERR_ENDSUBROUTINE;
				return -1;
			}
		;}
    break;

  case 896:

    {
		addOp(OP_REGEXMINIMAL);
	;}
    break;

  case 897:

    {
		addIntOp(OP_VAR_UN, varnumber[--nvarnumber]);
	;}
    break;

  case 898:

    {
		addIntOp(OP_ARR_UN, varnumber[--nvarnumber]);
	;}
    break;

  case 899:

    {
		addIntOp(OP_VARIABLEWATCH, varnumber[--nvarnumber]);
	;}
    break;

  case 900:

    {
		addOp(OP_IMAGECROP);
	;}
    break;

  case 901:

    {
		addIntOp(OP_PUSHINT,1);	// nr of arguments
		addOp(OP_IMAGEAUTOCROP);
	;}
    break;

  case 902:

    {
		addIntOp(OP_PUSHINT,2);	// nr of arguments
		addOp(OP_IMAGEAUTOCROP);
	;}
    break;

  case 903:

    {
		addIntOp(OP_PUSHINT,3);	// nr of arguments
		addOp(OP_IMAGERESIZE);
	;}
    break;

  case 904:

    {
		addIntOp(OP_PUSHINT,2);	// nr of arguments
		addOp(OP_IMAGERESIZE);
	;}
    break;

  case 905:

    {
		addOp(OP_IMAGESETPIXEL);
	;}
    break;

  case 906:

    {
		addOp(OP_GETCOLOR);
		addOp(OP_IMAGESETPIXEL);
	;}
    break;

  case 907:

    {
		addIntOp(OP_PUSHINT,6);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 908:

    {
		addIntOp(OP_PUSHINT,5);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 909:

    {
		addIntOp(OP_PUSHINT,4);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 910:

    {
		addIntOp(OP_PUSHINT,3);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 911:

    {
		addIntOp(OP_PUSHINT,6);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 912:

    {
		addIntOp(OP_PUSHINT,5);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 913:

    {
		addIntOp(OP_PUSHINT,4);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 914:

    {
		addIntOp(OP_PUSHINT,3);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 915:

    {
		addOp(OP_IMAGETRANSFORMED);
	;}
    break;

  case 916:

    {
		addIntOp(OP_PUSHINT,1); // opacity
		addOp(OP_IMAGETRANSFORMED);
	;}
    break;

  case 917:

    {
		addOp(OP_IMAGEROTATE);
	;}
    break;

  case 918:

    {
		addOp(OP_IMAGEFLIP);
	;}
    break;

  case 919:

    {
		addIntOp(OP_PUSHINT,0);
		addOp(OP_IMAGEFLIP);
	;}
    break;

  case 920:

    {
		addOp(OP_IMAGESMOOTH);
	;}
    break;

  case 921:

    {
		addOp(OP_UNLOAD);
	;}
    break;

  case 922:

    {
		addOp(OP_SETGRAPH);
	;}
    break;

  case 923:

    {
		addStringOp(OP_PUSHSTRING, "");
		addOp(OP_SETGRAPH);
	;}
    break;

  case 924:

    {
		addOp(OP_ARRAYBASE);
	;}
    break;



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





int
yyerror(const char *msg) {
        (void) msg;
        errorcode = COMPERR_SYNTAX;
	return -1;
}

