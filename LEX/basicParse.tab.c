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

		listlen = 0;
		listlenmax = 0;
		numberoflists = 0;

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
     B256GETARRAYBASE = 476,
     B256GETBRUSHCOLOR = 477,
     B256GETCLIPBOARDIMAGE = 478,
     B256GETCLIPBOARDSTRING = 479,
     B256GETCOLOR = 480,
     B256GETPENWIDTH = 481,
     B256GETSETTING = 482,
     B256GETSLICE = 483,
     B256GLOBAL = 484,
     B256GOSUB = 485,
     B256GOTO = 486,
     B256GRAPHHEIGHT = 487,
     B256GRAPHSIZE = 488,
     B256GRAPHVISIBLE = 489,
     B256GRAPHTOOLBARVISIBLE = 490,
     B256GRAPHWIDTH = 491,
     B256GREEN = 492,
     B256GREY = 493,
     B256GTE = 494,
     B256HOUR = 495,
     B256IF = 496,
     B256IN = 497,
     B256IMAGEAUTOCROP = 498,
     B256IMAGECENTERED = 499,
     B256IMAGECOPY = 500,
     B256IMAGECROP = 501,
     B256IMAGEDRAW = 502,
     B256IMAGEFLIP = 503,
     B256IMAGEHEIGHT = 504,
     B256IMAGELOAD = 505,
     B256IMAGENEW = 506,
     B256IMAGEPIXEL = 507,
     B256IMAGERESIZE = 508,
     B256IMAGEROTATE = 509,
     B256IMAGESETPIXEL = 510,
     B256IMAGESMOOTH = 511,
     B256IMAGETRANSFORMED = 512,
     B256IMAGETYPE_BMP = 513,
     B256IMAGETYPE_JPG = 514,
     B256IMAGETYPE_PNG = 515,
     B256IMAGEWIDTH = 516,
     B256IMGLOAD = 517,
     B256IMGSAVE = 518,
     B256IMPLODE = 519,
     B256INPUT = 520,
     B256INPUTFLOAT = 521,
     B256INPUTINT = 522,
     B256INPUTSTRING = 523,
     B256INSTR = 524,
     B256INSTRX = 525,
     B256INTDIV = 526,
     B256ISNUMERIC = 527,
     B256KEY = 528,
     B256KEYPRESSED = 529,
     B256KILL = 530,
     B256LASTERROR = 531,
     B256LASTERROREXTRA = 532,
     B256LASTERRORLINE = 533,
     B256LASTERRORMESSAGE = 534,
     B256LEFT = 535,
     B256LENGTH = 536,
     B256LET = 537,
     B256LINE = 538,
     B256LJUST = 539,
     B256LOG = 540,
     B256LOGTEN = 541,
     B256LOWER = 542,
     B256LTE = 543,
     B256LTRIM = 544,
     B256MAINTOOLBARVISIBLE = 545,
     B256MAP = 546,
     B256MD5 = 547,
     B256MID = 548,
     B256MIDX = 549,
     B256MINUTE = 550,
     B256MOD = 551,
     B256MONTH = 552,
     B256MOUSEB = 553,
     B256MOUSEBUTTON_CENTER = 554,
     B256MOUSEBUTTON_DOUBLECLICK = 555,
     B256MOUSEBUTTON_LEFT = 556,
     B256MOUSEBUTTON_NONE = 557,
     B256MOUSEBUTTON_RIGHT = 558,
     B256MOUSEX = 559,
     B256MOUSEY = 560,
     B256MSEC = 561,
     B256MULEQUAL = 562,
     B256NE = 563,
     B256NETADDRESS = 564,
     B256NETCLOSE = 565,
     B256NETCONNECT = 566,
     B256NETDATA = 567,
     B256NETLISTEN = 568,
     B256NETREAD = 569,
     B256NETWRITE = 570,
     B256NEXT = 571,
     B256NOT = 572,
     B256OFFERROR = 573,
     B256ONERROR = 574,
     B256OPEN = 575,
     B256OPENB = 576,
     B256OPENFILEDIALOG = 577,
     B256OPENSERIAL = 578,
     B256OR = 579,
     B256ORANGE = 580,
     B256OSTYPE = 581,
     B256OSTYPE_ANDROID = 582,
     B256OSTYPE_LINUX = 583,
     B256OSTYPE_MACINTOSH = 584,
     B256OSTYPE_WINDOWS = 585,
     B256OUTPUTVISIBLE = 586,
     B256OUTPUTTOOLBARVISIBLE = 587,
     B256PAUSE = 588,
     B256PENWIDTH = 589,
     B256PI = 590,
     B256PIE = 591,
     B256PIXEL = 592,
     B256PLOT = 593,
     B256POLY = 594,
     B256PORTIN = 595,
     B256PORTOUT = 596,
     B256PRINT = 597,
     B256PRINTERCANCEL = 598,
     B256PRINTEROFF = 599,
     B256PRINTERON = 600,
     B256PRINTERPAGE = 601,
     B256PROMPT = 602,
     B256PURPLE = 603,
     B256PUTSLICE = 604,
     B256RADIANS = 605,
     B256RAND = 606,
     B256READ = 607,
     B256READBYTE = 608,
     B256READLINE = 609,
     B256RECT = 610,
     B256RED = 611,
     B256REDIM = 612,
     B256REF = 613,
     B256REFRESH = 614,
     B256REGEXMINIMAL = 615,
     B256REM = 616,
     B256REPLACE = 617,
     B256REPLACEX = 618,
     B256SAVEFILEDIALOG = 619,
     B256RESET = 620,
     B256RETURN = 621,
     B256RGB = 622,
     B256RIGHT = 623,
     B256RJUST = 624,
     B256ROUND = 625,
     B256RTRIM = 626,
     B256SAY = 627,
     B256SECOND = 628,
     B256SEED = 629,
     B256SEEK = 630,
     B256SEMICOLON = 631,
     B256SEMICOLONEQUAL = 632,
     B256SERIALIZE = 633,
     B256SETCLIPBOARDIMAGE = 634,
     B256SETCLIPBOARDSTRING = 635,
     B256SETCOLOR = 636,
     B256SETGRAPH = 637,
     B256SETSETTING = 638,
     B256SIN = 639,
     B256SIZE = 640,
     B256SLICE_ALL = 641,
     B256SLICE_PAINT = 642,
     B256SLICE_SPRITE = 643,
     B256SOUND = 644,
     B256SOUNDENVELOPE = 645,
     B256SOUNDFADE = 646,
     B256SOUNDHARMONICS = 647,
     B256SOUNDID = 648,
     B256SOUNDLENGTH = 649,
     B256SOUNDLOAD = 650,
     B256SOUNDLOADRAW = 651,
     B256SOUNDLOOP = 652,
     B256SOUNDPAUSE = 653,
     B256SOUNDPLAY = 654,
     B256SOUNDPLAYER = 655,
     B256SOUNDPLAYEROFF = 656,
     B256SOUNDPOSITION = 657,
     B256SOUNDSAMPLERATE = 658,
     B256SOUNDSEEK = 659,
     B256SOUNDSTATE = 660,
     B256SOUNDSTOP = 661,
     B256SOUNDSYSTEM = 662,
     B256SOUNDVOLUME = 663,
     B256SOUNDWAIT = 664,
     B256SOUNDWAVEFORM = 665,
     B256SPRITECOLLIDE = 666,
     B256SPRITEDIM = 667,
     B256SPRITEH = 668,
     B256SPRITEHIDE = 669,
     B256SPRITELOAD = 670,
     B256SPRITEMOVE = 671,
     B256SPRITEO = 672,
     B256SPRITEPLACE = 673,
     B256SPRITEPOLY = 674,
     B256SPRITER = 675,
     B256SPRITES = 676,
     B256SPRITESHOW = 677,
     B256SPRITESLICE = 678,
     B256SPRITEV = 679,
     B256SPRITEW = 680,
     B256SPRITEX = 681,
     B256SPRITEY = 682,
     B256SQR = 683,
     B256STAMP = 684,
     B256STEP = 685,
     B256SUB1 = 686,
     B256SUBEQUAL = 687,
     B256SUBROUTINE = 688,
     B256SYSTEM = 689,
     B256TAN = 690,
     B256TEXT = 691,
     B256TEXTHEIGHT = 692,
     B256TEXTWIDTH = 693,
     B256THEN = 694,
     B256THROWERROR = 695,
     B256TO = 696,
     B256TOBINARY = 697,
     B256TOFLOAT = 698,
     B256TOHEX = 699,
     B256TOINT = 700,
     B256TOOCTAL = 701,
     B256TORADIX = 702,
     B256TOSTRING = 703,
     B256TRIM = 704,
     B256TRY = 705,
     B256TYPEOF = 706,
     B256TYPE_ARRAY = 707,
     B256TYPE_FLOAT = 708,
     B256TYPE_INT = 709,
     B256TYPE_MAP = 710,
     B256TYPE_REF = 711,
     B256TYPE_STRING = 712,
     B256TYPE_UNASSIGNED = 713,
     B256UNASSIGN = 714,
     B256UNLOAD = 715,
     B256UNSERIALIZE = 716,
     B256UNTIL = 717,
     B256UPPER = 718,
     B256VARIABLEWATCH = 719,
     B256VERSION = 720,
     B256VOLUME = 721,
     B256WARNING_ARRAYELEMENT = 722,
     B256WARNING_BOOLEANCONV = 723,
     B256WARNING_INTEGERRANGE = 724,
     B256WARNING_LONGRANGE = 725,
     B256WARNING_NUMBERCONV = 726,
     B256WARNING_REFNOTASSIGNED = 727,
     B256WARNING_SOUNDERROR = 728,
     B256WARNING_SOUNDFILEFORMAT = 729,
     B256WARNING_SOUNDLENGTH = 730,
     B256WARNING_SOUNDNOTSEEKABLE = 731,
     B256WARNING_START = 732,
     B256WARNING_STRING2NOTE = 733,
     B256WARNING_STRINGCONV = 734,
     B256WARNING_VARNOTASSIGNED = 735,
     B256WARNING_WAVOBSOLETE = 736,
     B256WAVLENGTH = 737,
     B256WAVPAUSE = 738,
     B256WAVPLAY = 739,
     B256WAVPOS = 740,
     B256WAVSEEK = 741,
     B256WAVSTATE = 742,
     B256WAVSTOP = 743,
     B256WAVWAIT = 744,
     B256WHILE = 745,
     B256WHITE = 746,
     B256WRITE = 747,
     B256WRITEBYTE = 748,
     B256WRITELINE = 749,
     B256XOR = 750,
     B256YEAR = 751,
     B256YELLOW = 752,
     B256ZFILL = 753,
     B256INTEGER = 754,
     B256FLOAT = 755,
     B256STRING = 756,
     B256HEXCONST = 757,
     B256BINCONST = 758,
     B256OCTCONST = 759,
     B256VARIABLE = 760,
     B256LABEL = 761,
     B256UNARY = 762
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
#define YYFINAL  934
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   37167

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  527
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  198
/* YYNRULES -- Number of rules.  */
#define YYNRULES  941
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1985

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   762

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     523,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   526,     2,     2,
     521,   522,   513,   512,   507,   511,     2,   514,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   524,     2,
     508,   510,   509,   525,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   517,     2,   518,   516,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   519,     2,   520,     2,     2,     2,     2,
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
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   515
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
     947,   951,   955,   958,   961,   964,   967,   970,   973,   976,
     979,   984,   989,   994,   999,  1006,  1015,  1026,  1033,  1042,
    1047,  1052,  1057,  1062,  1067,  1072,  1077,  1082,  1087,  1092,
    1097,  1102,  1107,  1112,  1117,  1120,  1123,  1126,  1131,  1136,
    1139,  1142,  1145,  1148,  1151,  1154,  1157,  1160,  1163,  1168,
    1171,  1176,  1179,  1184,  1187,  1190,  1193,  1196,  1199,  1202,
    1209,  1218,  1229,  1232,  1235,  1238,  1247,  1254,  1259,  1264,
    1269,  1274,  1279,  1284,  1289,  1294,  1297,  1302,  1309,  1314,
    1321,  1330,  1335,  1342,  1351,  1356,  1363,  1372,  1375,  1378,
    1381,  1386,  1391,  1398,  1407,  1414,  1417,  1420,  1425,  1432,
    1435,  1442,  1445,  1450,  1453,  1456,  1461,  1464,  1467,  1470,
    1475,  1482,  1487,  1492,  1497,  1504,  1506,  1508,  1510,  1513,
    1516,  1519,  1524,  1529,  1534,  1537,  1542,  1545,  1550,  1553,
    1558,  1561,  1564,  1569,  1574,  1579,  1584,  1589,  1594,  1599,
    1602,  1605,  1608,  1611,  1613,  1618,  1623,  1628,  1633,  1642,
    1649,  1658,  1665,  1672,  1675,  1680,  1683,  1688,  1691,  1696,
    1703,  1712,  1715,  1718,  1721,  1726,  1729,  1734,  1737,  1740,
    1747,  1752,  1755,  1764,  1775,  1784,  1789,  1794,  1799,  1804,
    1809,  1816,  1821,  1826,  1831,  1838,  1843,  1848,  1853,  1858,
    1863,  1866,  1869,  1872,  1877,  1882,  1887,  1896,  1903,  1908,
    1921,  1932,  1937,  1940,  1945,  1950,  1955,  1960,  1965,  1970,
    1975,  1977,  1979,  1981,  1986,  1989,  1992,  1995,  1998,  2001,
    2003,  2005,  2007,  2009,  2011,  2013,  2015,  2017,  2019,  2021,
    2023,  2025,  2027,  2029,  2031,  2033,  2035,  2037,  2039,  2041,
    2043,  2045,  2047,  2049,  2051,  2053,  2055,  2057,  2059,  2061,
    2063,  2065,  2067,  2069,  2071,  2073,  2075,  2077,  2079,  2081,
    2083,  2085,  2087,  2089,  2091,  2093,  2095,  2097,  2099,  2101,
    2103,  2105,  2107,  2109,  2111,  2113,  2115,  2117,  2119,  2121,
    2123,  2125,  2127,  2129,  2131,  2133,  2135,  2137,  2139,  2141,
    2143,  2145,  2147,  2149,  2151,  2153,  2155,  2157,  2159,  2161,
    2163,  2165,  2167,  2169,  2171,  2173,  2175,  2177,  2179,  2181,
    2183,  2185,  2187,  2189,  2191,  2193,  2195,  2197,  2199,  2201,
    2203,  2205,  2207,  2209,  2211,  2213,  2215,  2217,  2219,  2221,
    2223,  2225,  2227,  2229,  2231,  2233,  2235,  2237,  2239,  2241,
    2243,  2245,  2247,  2249,  2251,  2253,  2255,  2257,  2259,  2261,
    2263,  2265,  2267,  2269,  2271,  2273,  2275,  2277,  2279,  2281,
    2283,  2285,  2287,  2289,  2291,  2293,  2295,  2296,  2300,  2302,
    2304,  2306,  2308,  2310,  2312,  2314,  2318,  2321,  2324,  2328,
    2330,  2331,  2335,  2336,  2340,  2343,  2346,  2349,  2351,  2353,
    2355,  2358,  2361,  2366,  2370,  2376,  2380,  2386,  2391,  2396,
    2399,  2404,  2408,  2414,  2418,  2424,  2427,  2430,  2433,  2436,
    2439,  2442,  2445,  2448,  2451,  2454,  2458,  2461,  2464,  2468,
    2472,  2476,  2480,  2484,  2488,  2492,  2496,  2499,  2502,  2506,
    2510,  2514,  2518,  2522,  2526,  2533,  2542,  2547,  2555,  2558,
    2560,  2563,  2566,  2571,  2577,  2580,  2583,  2588,  2594,  2597,
    2600,  2603,  2606,  2609,  2612,  2615,  2618,  2621,  2624,  2627,
    2630,  2633,  2636,  2639,  2642,  2645,  2648,  2651,  2654,  2657,
    2660,  2663,  2666,  2669,  2672,  2675,  2678,  2681,  2684,  2687,
    2690,  2693,  2696,  2699,  2702,  2705,  2708,  2711,  2714,  2717,
    2720,  2723,  2726,  2729,  2732,  2735,  2738,  2741,  2744,  2747,
    2750,  2753,  2756,  2759,  2762,  2765,  2768,  2771,  2774,  2777,
    2780,  2783,  2786,  2789,  2792,  2795,  2798,  2801,  2804,  2807,
    2810,  2813,  2816,  2819,  2822,  2825,  2828,  2831,  2834,  2837,
    2840,  2843,  2846,  2849,  2852,  2855,  2858,  2861,  2864,  2867,
    2870,  2873,  2876,  2879,  2882,  2885,  2888,  2891,  2894,  2897,
    2900,  2904,  2907,  2912,  2915,  2919,  2922,  2927,  2930,  2933,
    2936,  2939,  2942,  2945,  2948,  2951,  2954,  2957,  2960,  2963,
    2966,  2969,  2972,  2975,  2978,  2981,  2984,  2987,  2990,  2993,
    2996,  2999,  3002,  3005,  3008,  3011,  3014,  3017,  3020,  3023,
    3026,  3029,  3032,  3035,  3038,  3041,  3044,  3047,  3050,  3053,
    3056,  3059,  3062,  3065,  3068,  3071,  3074,  3077,  3080,  3083,
    3086,  3089,  3092,  3095,  3098,  3101,  3104,  3107,  3110,  3113,
    3115,  3117,  3119,  3121,  3123,  3125,  3128,  3131,  3134,  3137,
    3141,  3145,  3147,  3149,  3152,  3155,  3158,  3161,  3164,  3167,
    3170,  3173,  3176,  3179,  3182,  3185,  3188,  3191,  3194,  3197,
    3200,  3203,  3206,  3209,  3212,  3215,  3218,  3221,  3224,  3227,
    3230,  3233
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     528,     0,    -1,   530,   529,   528,    -1,   530,    -1,   523,
      -1,   531,   532,    -1,   531,   532,   361,    -1,   532,    -1,
     532,   361,    -1,   531,    -1,   531,   361,    -1,   361,    -1,
      -1,   506,    -1,   571,   524,   532,    -1,   571,    -1,   517,
     518,    -1,   517,   525,   518,    -1,   517,   525,   507,   518,
      -1,   534,   533,    -1,   517,   507,   525,   518,    -1,   533,
     534,    -1,   517,   563,   507,   563,   518,    -1,   517,   563,
     518,   517,   563,   518,    -1,   517,   563,   518,    -1,   505,
      -1,   521,   538,   522,    -1,   538,    -1,   538,   533,    -1,
     521,   539,   522,    -1,   538,   537,    -1,   521,   540,   522,
      -1,   539,    -1,   521,   522,    -1,   521,   543,   522,    -1,
     541,    -1,   541,   507,   543,    -1,   519,   545,   520,    -1,
     547,    -1,   546,    -1,   546,   507,   545,    -1,   519,   547,
     520,    -1,   563,    -1,   563,   507,   547,    -1,   519,   549,
     520,    -1,   563,   511,   509,   563,    -1,   563,   511,   509,
     563,   507,   549,    -1,   551,    -1,   551,   507,   550,    -1,
     563,    -1,   358,   521,   538,   522,    -1,   521,   522,    -1,
      -1,   563,   507,   563,    -1,   521,   553,   522,    -1,   563,
     507,   540,    -1,   521,   554,   522,    -1,   563,   507,   538,
      -1,   521,   555,   522,    -1,   563,   507,   563,   507,   563,
      -1,   521,   556,   522,    -1,   563,   507,   563,   507,   563,
     507,   563,    -1,   521,   557,   522,    -1,   563,   507,   563,
     507,   563,   507,   563,   507,   563,    -1,   521,   558,   522,
      -1,   563,   507,   563,   507,   563,   507,   563,   507,   563,
     507,   563,    -1,   521,   559,   522,    -1,   563,   507,   563,
     507,   563,   507,   563,   507,   563,   507,   563,   507,   563,
      -1,   521,   560,   522,    -1,   563,   507,   563,   507,   563,
     507,   563,   507,   563,   507,   563,   507,   563,   507,   563,
     507,   563,    -1,   521,   561,   522,    -1,   563,   507,   563,
     507,   563,   507,   563,   507,   563,   507,   563,   507,   563,
     507,   563,   507,   563,   507,   563,    -1,   521,   562,   522,
      -1,   564,    -1,   565,    -1,   566,    -1,   567,    -1,   568,
      -1,   569,    -1,   570,    -1,   540,    -1,   521,   563,   522,
      -1,   563,   512,   563,    -1,   563,   376,   563,    -1,   563,
       8,   563,    -1,   563,   513,   563,    -1,   538,   521,   550,
     522,    -1,   538,   521,   522,    -1,    22,   552,    -1,    23,
     552,    -1,    25,   552,    -1,    26,   552,    -1,    35,   552,
      -1,    51,   552,    -1,    52,   552,    -1,    53,   552,    -1,
      54,   552,    -1,    55,   552,    -1,    56,   552,    -1,    57,
     552,    -1,    58,   552,    -1,    59,   552,    -1,   237,   552,
      -1,   238,   552,    -1,   299,   552,    -1,   300,   552,    -1,
     301,   552,    -1,   302,   552,    -1,   303,   552,    -1,   325,
     552,    -1,   327,   552,    -1,   328,   552,    -1,   329,   552,
      -1,   330,   552,    -1,   348,   552,    -1,   356,   552,    -1,
     386,   552,    -1,   387,   552,    -1,   388,   552,    -1,   452,
     552,    -1,   453,   552,    -1,   454,   552,    -1,   455,   552,
      -1,   456,   552,    -1,   457,   552,    -1,   458,   552,    -1,
     491,   552,    -1,   497,   552,    -1,    87,   552,    -1,    88,
     552,    -1,    89,   552,    -1,    90,   552,    -1,    91,   552,
      -1,    92,   552,    -1,    93,   552,    -1,    94,   552,    -1,
      95,   552,    -1,    96,   552,    -1,    97,   552,    -1,    98,
     552,    -1,    99,   552,    -1,   100,   552,    -1,   101,   552,
      -1,   102,   552,    -1,   103,   552,    -1,   104,   552,    -1,
     105,   552,    -1,   106,   552,    -1,   107,   552,    -1,   108,
     552,    -1,   109,   552,    -1,   110,   552,    -1,   111,   552,
      -1,   112,   552,    -1,   113,   552,    -1,   114,   552,    -1,
     115,   552,    -1,   116,   552,    -1,   117,   552,    -1,   118,
     552,    -1,   119,   552,    -1,   120,   552,    -1,   121,   552,
      -1,   122,   552,    -1,   123,   552,    -1,   124,   552,    -1,
     125,   552,    -1,   126,   552,    -1,   127,   552,    -1,   128,
     552,    -1,   129,   552,    -1,   130,   552,    -1,   131,   552,
      -1,   132,   552,    -1,   133,   552,    -1,   134,   552,    -1,
     135,   552,    -1,   136,   552,    -1,   137,   552,    -1,   138,
     552,    -1,   139,   552,    -1,   140,   552,    -1,   141,   552,
      -1,   142,   552,    -1,   143,   552,    -1,   144,   552,    -1,
     145,   552,    -1,   146,   552,    -1,   147,   552,    -1,   148,
     552,    -1,   149,   552,    -1,   150,   552,    -1,   151,   552,
      -1,   152,   552,    -1,   153,   552,    -1,   154,   552,    -1,
     155,   552,    -1,   156,   552,    -1,   157,   552,    -1,   158,
     552,    -1,   159,   552,    -1,   160,   552,    -1,   161,   552,
      -1,   162,   552,    -1,   163,   552,    -1,   164,   552,    -1,
     165,   552,    -1,   166,   552,    -1,   167,   552,    -1,   168,
     552,    -1,   169,   552,    -1,   170,   552,    -1,   171,   552,
      -1,   172,   552,    -1,   173,   552,    -1,   174,   552,    -1,
     175,   552,    -1,   176,   552,    -1,   177,   552,    -1,   178,
     552,    -1,   179,   552,    -1,   180,   552,    -1,   181,   552,
      -1,   182,   552,    -1,   183,   552,    -1,   184,   552,    -1,
     185,   552,    -1,   186,   552,    -1,   187,   552,    -1,   188,
     552,    -1,   189,   552,    -1,   190,   552,    -1,   191,   552,
      -1,   192,   552,    -1,   193,   552,    -1,   194,   552,    -1,
     195,   552,    -1,   196,   552,    -1,   197,   552,    -1,   198,
     552,    -1,   467,   552,    -1,   468,   552,    -1,   469,   552,
      -1,   470,   552,    -1,   471,   552,    -1,   472,   552,    -1,
     473,   552,    -1,   474,   552,    -1,   475,   552,    -1,   476,
     552,    -1,   477,   552,    -1,   478,   552,    -1,   479,   552,
      -1,   480,   552,    -1,   481,   552,    -1,   499,    -1,   500,
      -1,   512,   499,    -1,   512,   500,    -1,   511,   563,    -1,
     563,   511,   563,    -1,   563,   296,   563,    -1,   563,   526,
     563,    -1,   563,   526,    -1,   563,   271,   563,    -1,   563,
     514,   563,    -1,   563,   516,   563,    -1,   563,    19,   563,
      -1,   563,    20,   563,    -1,   563,    21,   563,    -1,    18,
     563,    -1,   563,    10,   563,    -1,   563,   324,   563,    -1,
     563,   495,   563,    -1,   317,   563,    -1,   563,   510,   563,
      -1,   563,   308,   563,    -1,   563,   508,   563,    -1,   563,
     509,   563,    -1,   563,   239,   563,    -1,   563,   288,   563,
      -1,   563,   242,   563,    -1,   540,     5,    -1,   540,   431,
      -1,     5,   540,    -1,   431,   540,    -1,   538,     5,    -1,
     538,   431,    -1,     5,   538,    -1,   431,   538,    -1,   445,
     521,   563,   522,    -1,   443,   521,   563,   522,    -1,   281,
     521,   563,   522,    -1,    13,   521,   563,   522,    -1,   269,
     521,   563,   507,   563,   522,    -1,   269,   521,   563,   507,
     563,   507,   563,   522,    -1,   269,   521,   563,   507,   563,
     507,   563,   507,   563,   522,    -1,   270,   521,   563,   507,
     563,   522,    -1,   270,   521,   563,   507,   563,   507,   563,
     522,    -1,    30,   521,   563,   522,    -1,   208,   521,   563,
     522,    -1,   384,   521,   563,   522,    -1,    47,   521,   563,
     522,    -1,   435,   521,   563,   522,    -1,    14,   521,   563,
     522,    -1,     4,   521,   563,   522,    -1,    16,   521,   563,
     522,    -1,    72,   521,   563,   522,    -1,   350,   521,   563,
     522,    -1,   285,   521,   563,   522,    -1,   286,   521,   563,
     522,    -1,   428,   521,   563,   522,    -1,   203,   521,   563,
     522,    -1,     3,   521,   563,   522,    -1,   351,   552,    -1,
     335,   552,    -1,    24,   552,    -1,    24,   521,   563,   522,
      -1,   199,   521,   563,   522,    -1,   496,   552,    -1,   297,
     552,    -1,    60,   552,    -1,   240,   552,    -1,   295,   552,
      -1,   373,   552,    -1,   236,   552,    -1,   232,   552,    -1,
     385,   552,    -1,   385,   521,   563,   522,    -1,   274,   552,
      -1,   274,   521,   563,   522,    -1,   273,   552,    -1,   273,
     521,   563,   522,    -1,   304,   552,    -1,   305,   552,    -1,
     298,   552,    -1,    39,   552,    -1,    40,   552,    -1,    37,
     552,    -1,   337,   521,   563,   507,   563,   522,    -1,   367,
     521,   563,   507,   563,   507,   563,   522,    -1,   367,   521,
     563,   507,   563,   507,   563,   507,   563,   522,    -1,   225,
     552,    -1,   222,   552,    -1,   226,   552,    -1,   411,   521,
     563,   507,   563,   507,   563,   522,    -1,   411,   521,   563,
     507,   563,   522,    -1,   426,   521,   563,   522,    -1,   427,
     521,   563,   522,    -1,   413,   521,   563,   522,    -1,   425,
     521,   563,   522,    -1,   424,   521,   563,   522,    -1,   420,
     521,   563,   522,    -1,   421,   521,   563,   522,    -1,   417,
     521,   563,   522,    -1,    69,   552,    -1,    69,   521,   563,
     522,    -1,    69,   521,   563,   507,   563,   522,    -1,    65,
     521,   563,   522,    -1,    65,   521,   563,   507,   563,   522,
      -1,    65,   521,   563,   507,   563,   507,   563,   522,    -1,
      64,   521,   563,   522,    -1,    64,   521,   563,   507,   563,
     522,    -1,    64,   521,   563,   507,   563,   507,   563,   522,
      -1,    66,   521,   563,   522,    -1,    66,   521,   563,   507,
     563,   522,    -1,    66,   521,   563,   507,   563,   507,   563,
     522,    -1,   276,   552,    -1,   278,   552,    -1,   312,   552,
      -1,   312,   521,   563,   522,    -1,   340,   521,   563,   522,
      -1,    48,   521,   563,   507,   563,   522,    -1,    48,   521,
     563,   507,   563,   507,   563,   522,    -1,    49,   521,   563,
     507,   563,   522,    -1,   326,   552,    -1,   306,   552,    -1,
     438,   521,   563,   522,    -1,   438,   521,   563,   507,   563,
     522,    -1,   437,   552,    -1,   437,   521,   563,   507,   563,
     522,    -1,   353,   552,    -1,   353,   521,   563,   522,    -1,
     212,   552,    -1,   213,   552,    -1,   213,   521,   563,   522,
      -1,   214,   552,    -1,   215,   552,    -1,   465,   552,    -1,
      43,   521,   563,   522,    -1,    43,   521,   563,   507,   563,
     522,    -1,   216,   521,   563,   522,    -1,   217,   521,   563,
     522,    -1,   218,   521,   563,   522,    -1,   219,   521,   563,
     507,   563,   522,    -1,   503,    -1,   502,    -1,   504,    -1,
     482,   552,    -1,   485,   552,    -1,   487,   552,    -1,   451,
     521,   563,   522,    -1,   400,   521,   563,   522,    -1,   400,
     521,   553,   522,    -1,   393,   552,    -1,   402,   521,   563,
     522,    -1,   402,   552,    -1,   405,   521,   563,   522,    -1,
     405,   552,    -1,   394,   521,   563,   522,    -1,   394,   552,
      -1,   403,   552,    -1,   261,   521,   563,   522,    -1,   249,
     521,   563,   522,    -1,   252,   521,   556,   522,    -1,   370,
     521,   563,   522,    -1,   370,   521,   553,   522,    -1,    15,
     521,   539,   522,    -1,    15,   521,   540,   522,    -1,   538,
     534,    -1,   538,   535,    -1,   538,   536,    -1,   221,   552,
      -1,   501,    -1,    33,   521,   563,   522,    -1,   448,   521,
     563,   522,    -1,   463,   521,   563,   522,    -1,   287,   521,
     563,   522,    -1,   293,   521,   563,   507,   563,   507,   563,
     522,    -1,   294,   521,   563,   507,   563,   522,    -1,   294,
     521,   563,   507,   563,   507,   563,   522,    -1,   280,   521,
     563,   507,   563,   522,    -1,   368,   521,   563,   507,   563,
     522,    -1,   352,   552,    -1,   352,   521,   563,   522,    -1,
     354,   552,    -1,   354,   521,   563,   522,    -1,    50,   552,
      -1,    70,   521,   563,   522,    -1,    70,   521,   563,   507,
     563,   522,    -1,    70,   521,   563,   507,   563,   507,   563,
     522,    -1,   279,   552,    -1,   277,   552,    -1,   314,   552,
      -1,   314,   521,   563,   522,    -1,   309,   552,    -1,   292,
     521,   563,   522,    -1,   223,   552,    -1,   224,   552,    -1,
     227,   521,   563,   507,   563,   522,    -1,    74,   521,   563,
     522,    -1,    74,   552,    -1,   362,   521,   563,   507,   563,
     507,   563,   522,    -1,   362,   521,   563,   507,   563,   507,
     563,   507,   563,   522,    -1,   363,   521,   563,   507,   563,
     507,   563,   522,    -1,   378,   521,   563,   522,    -1,   264,
     521,   563,   522,    -1,   264,   521,   553,   522,    -1,   264,
     521,   556,   522,    -1,   347,   521,   563,   522,    -1,   347,
     521,   563,   507,   563,   522,    -1,   442,   521,   563,   522,
      -1,   444,   521,   563,   522,    -1,   446,   521,   563,   522,
      -1,   447,   521,   563,   507,   563,   522,    -1,    71,   521,
     563,   522,    -1,   272,   521,   563,   522,    -1,   289,   521,
     563,   522,    -1,   371,   521,   563,   522,    -1,   449,   521,
     563,   522,    -1,   258,   552,    -1,   259,   552,    -1,   260,
     552,    -1,   395,   521,   563,   522,    -1,   395,   521,   553,
     522,    -1,   396,   521,   563,   522,    -1,   251,   521,   563,
     507,   563,   507,   563,   522,    -1,   251,   521,   563,   507,
     563,   522,    -1,   250,   521,   563,   522,    -1,   245,   521,
     563,   507,   563,   507,   563,   507,   563,   507,   563,   522,
      -1,   245,   521,   563,   507,   563,   507,   563,   507,   563,
     522,    -1,   245,   521,   563,   522,    -1,   245,   552,    -1,
     284,   521,   553,   522,    -1,   284,   521,   556,   522,    -1,
     369,   521,   553,   522,    -1,   369,   521,   556,   522,    -1,
     498,   521,   553,   522,    -1,   322,   521,   556,   522,    -1,
     364,   521,   556,   522,    -1,   539,    -1,   544,    -1,   548,
      -1,   461,   521,   563,   522,    -1,   204,   553,    -1,   204,
     556,    -1,   205,   553,    -1,   228,   557,    -1,   228,   558,
      -1,   694,    -1,   633,    -1,   724,    -1,   572,    -1,   610,
      -1,   573,    -1,   575,    -1,   670,    -1,   634,    -1,   631,
      -1,   597,    -1,   669,    -1,   648,    -1,   614,    -1,   695,
      -1,   696,    -1,   697,    -1,   675,    -1,   672,    -1,   673,
      -1,   674,    -1,   671,    -1,   593,    -1,   576,    -1,   723,
      -1,   686,    -1,   632,    -1,   577,    -1,   578,    -1,   707,
      -1,   579,    -1,   601,    -1,   708,    -1,   580,    -1,   581,
      -1,   698,    -1,   699,    -1,   700,    -1,   598,    -1,   638,
      -1,   605,    -1,   606,    -1,   705,    -1,   692,    -1,   609,
      -1,   608,    -1,   599,    -1,   687,    -1,   690,    -1,   582,
      -1,   583,    -1,   584,    -1,   713,    -1,   717,    -1,   712,
      -1,   716,    -1,   720,    -1,   715,    -1,   714,    -1,   719,
      -1,   721,    -1,   718,    -1,   660,    -1,   685,    -1,   652,
      -1,   680,    -1,   591,    -1,   630,    -1,   689,    -1,   592,
      -1,   679,    -1,   677,    -1,   676,    -1,   678,    -1,   607,
      -1,   611,    -1,   612,    -1,   644,    -1,   688,    -1,   691,
      -1,   595,    -1,   693,    -1,   635,    -1,   629,    -1,   642,
      -1,   684,    -1,   701,    -1,   702,    -1,   703,    -1,   704,
      -1,   653,    -1,   659,    -1,   636,    -1,   594,    -1,   600,
      -1,   709,    -1,   649,    -1,   613,    -1,   639,    -1,   650,
      -1,   651,    -1,   681,    -1,   682,    -1,   683,    -1,   615,
      -1,   617,    -1,   618,    -1,   616,    -1,   619,    -1,   620,
      -1,   621,    -1,   622,    -1,   623,    -1,   624,    -1,   625,
      -1,   626,    -1,   627,    -1,   628,    -1,   661,    -1,   667,
      -1,   662,    -1,   666,    -1,   665,    -1,   664,    -1,   668,
      -1,   663,    -1,   643,    -1,   706,    -1,   640,    -1,   637,
      -1,   596,    -1,   586,    -1,   710,    -1,   722,    -1,   587,
      -1,   711,    -1,   641,    -1,   654,    -1,   655,    -1,   656,
      -1,   657,    -1,   658,    -1,   589,    -1,   647,    -1,   646,
      -1,   645,    -1,    17,    -1,    -1,    28,   574,   563,    -1,
      29,    -1,    76,    -1,    79,    -1,    81,    -1,    83,    -1,
      85,    -1,    86,    -1,   241,   563,   439,    -1,   582,   532,
      -1,   585,   532,    -1,   582,   532,    79,    -1,   450,    -1,
      -1,   462,   588,   563,    -1,    -1,   490,   590,   563,    -1,
     282,   604,    -1,   282,   603,    -1,   282,   602,    -1,   604,
      -1,   603,    -1,   602,    -1,   291,   543,    -1,    73,   540,
      -1,    73,   540,   207,   563,    -1,    73,   539,   563,    -1,
      73,   539,   563,   207,   563,    -1,    73,   539,   553,    -1,
      73,   539,   553,   207,   563,    -1,    73,   539,   510,   563,
      -1,    73,   539,   207,   563,    -1,   357,   540,    -1,   357,
     540,   207,   563,    -1,   357,   539,   563,    -1,   357,   539,
     563,   207,   563,    -1,   357,   539,   553,    -1,   357,   539,
     553,   207,   563,    -1,   333,   563,    -1,   440,   563,    -1,
      42,   552,    -1,    36,   552,    -1,    36,   563,    -1,   206,
     552,    -1,   233,   553,    -1,   233,   556,    -1,   359,   552,
      -1,    80,   552,    -1,   540,   510,   563,    -1,   540,     5,
      -1,   540,   431,    -1,   540,     6,   563,    -1,   540,   432,
     563,    -1,   540,   307,   563,    -1,   540,    75,   563,    -1,
     540,     9,   563,    -1,   540,   377,   563,    -1,   539,   207,
     563,    -1,   539,   510,   563,    -1,   538,     5,    -1,   538,
     431,    -1,   538,     6,   563,    -1,   538,   432,   563,    -1,
     538,   307,   563,    -1,   538,    75,   563,    -1,   538,     9,
     563,    -1,   538,   377,   563,    -1,   210,   538,   510,   563,
     441,   563,    -1,   210,   538,   510,   563,   441,   563,   430,
     563,    -1,   211,   538,   242,   570,    -1,   211,   538,   511,
     509,   538,   242,   570,    -1,   316,   538,    -1,   316,    -1,
     231,   538,    -1,   230,   538,    -1,    27,   538,   521,   522,
      -1,    27,   538,   521,   550,   522,    -1,   318,   552,    -1,
     319,   538,    -1,   319,   538,   521,   522,    -1,   319,   538,
     521,   550,   522,    -1,   366,   552,    -1,   366,   563,    -1,
     381,   563,    -1,   381,   553,    -1,   381,   556,    -1,   389,
     553,    -1,   389,   563,    -1,   399,   552,    -1,   399,   553,
      -1,   399,   563,    -1,   398,   563,    -1,   398,   552,    -1,
     401,   563,    -1,   401,   552,    -1,   406,   563,    -1,   406,
     552,    -1,   409,   563,    -1,   409,   552,    -1,   410,   563,
      -1,   410,   553,    -1,   407,   563,    -1,   390,   552,    -1,
     390,   557,    -1,   390,   563,    -1,   392,   552,    -1,   392,
     553,    -1,   392,   563,    -1,   391,   557,    -1,   391,   556,
      -1,   404,   553,    -1,   404,   563,    -1,   408,   553,    -1,
     408,   563,    -1,   397,   553,    -1,   397,   563,    -1,   338,
     553,    -1,   283,   557,    -1,    34,   556,    -1,    78,   557,
      -1,    11,   558,    -1,    11,   559,    -1,    32,   558,    -1,
      32,   559,    -1,   336,   558,    -1,   336,   559,    -1,   355,
     557,    -1,   355,   558,    -1,   355,   559,    -1,   436,   556,
      -1,   436,   558,    -1,   436,   559,    -1,   209,   557,    -1,
     209,   556,    -1,   209,   553,    -1,   209,   563,    -1,   372,
     563,    -1,   434,   563,    -1,   466,   563,    -1,   339,   563,
      -1,   429,   558,    -1,   429,   557,    -1,   429,   556,    -1,
     320,   563,    -1,   320,   553,    -1,   321,   563,    -1,   321,
     553,    -1,   323,   553,    -1,   323,   556,    -1,   323,   557,
      -1,   323,   558,    -1,   323,   559,    -1,   323,   560,    -1,
     492,   563,    -1,   492,   553,    -1,   494,   563,    -1,   494,
     553,    -1,   493,   563,    -1,   493,   553,    -1,    41,   552,
      -1,    41,   563,    -1,   365,   552,    -1,   365,   563,    -1,
     374,   563,    -1,   375,   563,    -1,   375,   553,    -1,   265,
     555,    -1,   265,   538,    -1,   265,   554,    -1,   265,   540,
      -1,   268,   555,    -1,   268,   538,    -1,   268,   554,    -1,
     268,   540,    -1,   267,   555,    -1,   267,   538,    -1,   267,
     554,    -1,   267,   540,    -1,   266,   555,    -1,   266,   538,
      -1,   266,   554,    -1,   266,   540,    -1,   342,   552,    -1,
     342,   563,   376,    -1,   342,   547,    -1,   342,   521,   547,
     522,    -1,   525,   552,    -1,   525,   563,   376,    -1,   525,
     547,    -1,   525,   521,   547,   522,    -1,   483,   552,    -1,
     484,   552,    -1,   484,   563,    -1,   486,   563,    -1,   488,
     552,    -1,   489,   552,    -1,   349,   556,    -1,   262,   556,
      -1,   262,   557,    -1,   262,   558,    -1,   412,   563,    -1,
     415,   553,    -1,   423,   558,    -1,   419,   553,    -1,   418,
     556,    -1,   418,   557,    -1,   418,   558,    -1,   418,   559,
      -1,   416,   556,    -1,   416,   557,    -1,   416,   558,    -1,
     416,   559,    -1,   414,   563,    -1,   422,   563,    -1,    38,
     552,    -1,    31,   563,    -1,    67,   563,    -1,    67,   553,
      -1,    61,   552,    -1,    61,   563,    -1,    63,   563,    -1,
      63,   553,    -1,    68,   563,    -1,    68,   553,    -1,    68,
     556,    -1,    62,   552,    -1,    62,   563,    -1,    62,   553,
      -1,   313,   563,    -1,   313,   553,    -1,   311,   553,    -1,
     311,   556,    -1,   315,   563,    -1,   315,   553,    -1,   310,
     552,    -1,   310,   563,    -1,   275,   563,    -1,   379,   563,
      -1,   380,   563,    -1,   383,   556,    -1,   341,   553,    -1,
     263,   563,    -1,   263,   553,    -1,    77,   563,    -1,   234,
     563,    -1,   331,   563,    -1,   290,   563,    -1,   235,   563,
      -1,   332,   563,    -1,   229,   543,    -1,   334,   563,    -1,
       7,   563,    -1,    44,    -1,    45,    -1,    46,    -1,   200,
      -1,   201,    -1,   202,    -1,   343,   552,    -1,   344,   552,
      -1,   345,   552,    -1,   346,   552,    -1,   220,   538,   542,
      -1,   433,   538,   542,    -1,    82,    -1,    84,    -1,   360,
     563,    -1,   459,   539,    -1,   459,   540,    -1,   464,   539,
      -1,   246,   558,    -1,   243,   563,    -1,   243,   553,    -1,
     253,   556,    -1,   253,   553,    -1,   255,   557,    -1,   255,
     556,    -1,   247,   559,    -1,   247,   558,    -1,   247,   557,
      -1,   247,   556,    -1,   244,   559,    -1,   244,   558,    -1,
     244,   557,    -1,   244,   556,    -1,   257,   562,    -1,   257,
     561,    -1,   254,   553,    -1,   248,   556,    -1,   248,   553,
      -1,   256,   563,    -1,   460,   563,    -1,   382,   563,    -1,
     382,   552,    -1,    12,   563,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   896,   896,   897,   901,   909,   910,   911,   912,   913,
     914,   915,   916,   920,   938,   939,   952,   956,   960,   961,
     965,   966,   971,   972,   973,   980,   983,   987,   988,   989,
     993,   994,  1004,  1011,  1012,  1017,  1018,  1031,  1043,  1049,
    1050,  1056,  1065,  1066,  1072,  1083,  1084,  1090,  1091,  1097,
    1098,  1112,  1113,  1120,  1121,  1125,  1126,  1130,  1131,  1136,
    1137,  1142,  1143,  1148,  1149,  1154,  1155,  1160,  1161,  1166,
    1167,  1172,  1173,  1178,  1179,  1180,  1181,  1182,  1183,  1184,
    1185,  1195,  1196,  1199,  1202,  1205,  1213,  1219,  1234,  1235,
    1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,
    1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,
    1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1281,  1284,
    1287,  1290,  1293,  1296,  1299,  1302,  1305,  1308,  1311,  1314,
    1317,  1320,  1323,  1326,  1329,  1332,  1335,  1338,  1341,  1344,
    1347,  1350,  1353,  1356,  1359,  1362,  1365,  1368,  1371,  1374,
    1377,  1380,  1383,  1386,  1389,  1392,  1395,  1398,  1401,  1404,
    1407,  1410,  1413,  1416,  1419,  1422,  1425,  1428,  1431,  1434,
    1437,  1440,  1443,  1446,  1449,  1452,  1455,  1458,  1461,  1464,
    1467,  1470,  1473,  1476,  1479,  1482,  1485,  1488,  1491,  1494,
    1497,  1500,  1503,  1506,  1509,  1512,  1515,  1518,  1521,  1524,
    1527,  1530,  1533,  1536,  1539,  1542,  1545,  1548,  1551,  1554,
    1557,  1560,  1563,  1566,  1569,  1572,  1575,  1578,  1581,  1584,
    1587,  1590,  1593,  1596,  1599,  1602,  1605,  1608,  1611,  1614,
    1617,  1620,  1623,  1626,  1629,  1632,  1635,  1638,  1641,  1644,
    1647,  1650,  1653,  1656,  1659,  1669,  1670,  1679,  1684,  1693,
    1697,  1700,  1703,  1706,  1711,  1714,  1717,  1718,  1719,  1720,
    1721,  1722,  1725,  1728,  1729,  1730,  1731,  1732,  1733,  1734,
    1735,  1736,  1740,  1752,  1764,  1776,  1788,  1795,  1802,  1809,
    1816,  1817,  1818,  1819,  1820,  1825,  1829,  1830,  1834,  1835,
    1836,  1837,  1838,  1839,  1840,  1841,  1842,  1843,  1844,  1845,
    1846,  1847,  1848,  1849,  1850,  1851,  1852,  1856,  1857,  1858,
    1859,  1860,  1861,  1862,  1863,  1864,  1865,  1866,  1870,  1871,
    1875,  1876,  1880,  1883,  1884,  1885,  1886,  1887,  1888,  1889,
    1890,  1894,  1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,
    1905,  1906,  1907,  1908,  1909,  1910,  1915,  1919,  1922,  1928,
    1932,  1934,  1940,  1944,  1946,  1952,  1956,  1958,  1959,  1960,
    1961,  1962,  1963,  1967,  1968,  1969,  1970,  1971,  1972,  1973,
    1974,  1975,  1976,  1977,  1978,  1982,  1983,  1984,  1985,  1986,
    1990,  1993,  1997,  2001,  2005,  2008,  2018,  2028,  2038,  2039,
    2040,  2042,  2046,  2049,  2055,  2058,  2061,  2065,  2068,  2072,
    2075,  2079,  2082,  2085,  2088,  2091,  2095,  2098,  2101,  2104,
    2107,  2110,  2113,  2121,  2122,  2123,  2124,  2125,  2126,  2127,
    2128,  2129,  2130,  2131,  2132,  2133,  2134,  2135,  2136,  2142,
    2146,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,
    2157,  2158,  2159,  2163,  2164,  2165,  2168,  2173,  2177,  2180,
    2183,  2185,  2189,  2193,  2197,  2200,  2203,  2204,  2205,  2206,
    2207,  2208,  2209,  2210,  2213,  2219,  2222,  2225,  2229,  2232,
    2236,  2240,  2244,  2248,  2252,  2255,  2259,  2262,  2266,  2269,
    2283,  2286,  2289,  2292,  2296,  2300,  2303,  2307,  2311,  2322,
    2323,  2324,  2325,  2326,  2327,  2328,  2329,  2330,  2331,  2332,
    2333,  2334,  2335,  2336,  2337,  2338,  2339,  2340,  2341,  2342,
    2343,  2344,  2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,
    2353,  2354,  2355,  2356,  2357,  2358,  2359,  2360,  2361,  2362,
    2363,  2364,  2365,  2366,  2367,  2368,  2369,  2370,  2371,  2372,
    2373,  2374,  2375,  2376,  2377,  2378,  2379,  2380,  2381,  2382,
    2383,  2384,  2385,  2386,  2387,  2388,  2389,  2390,  2391,  2392,
    2393,  2394,  2395,  2396,  2397,  2398,  2399,  2400,  2401,  2402,
    2403,  2404,  2405,  2406,  2407,  2408,  2409,  2410,  2411,  2412,
    2413,  2414,  2415,  2416,  2417,  2418,  2419,  2420,  2421,  2422,
    2423,  2424,  2425,  2426,  2427,  2428,  2429,  2430,  2431,  2432,
    2433,  2434,  2435,  2436,  2437,  2438,  2439,  2440,  2441,  2442,
    2443,  2444,  2445,  2446,  2447,  2448,  2449,  2450,  2451,  2452,
    2453,  2454,  2455,  2456,  2457,  2458,  2459,  2460,  2461,  2462,
    2463,  2464,  2465,  2466,  2467,  2472,  2479,  2479,  2510,  2537,
    2547,  2593,  2624,  2646,  2669,  2693,  2704,  2715,  2724,  2738,
    2749,  2749,  2776,  2776,  2787,  2788,  2789,  2790,  2791,  2792,
    2795,  2802,  2805,  2811,  2816,  2824,  2827,  2833,  2836,  2842,
    2845,  2851,  2856,  2864,  2867,  2875,  2881,  2886,  2889,  2899,
    2905,  2911,  2915,  2921,  2926,  2933,  2936,  2945,  2954,  2964,
    2975,  2985,  2996,  3007,  3022,  3030,  3033,  3039,  3045,  3051,
    3057,  3062,  3068,  3074,  3083,  3094,  3106,  3115,  3127,  3149,
    3169,  3178,  3188,  3193,  3201,  3207,  3210,  3213,  3219,  3231,
    3243,  3247,  3250,  3259,  3266,  3271,  3275,  3281,  3289,  3292,
    3298,  3301,  3307,  3310,  3317,  3320,  3327,  3331,  3336,  3341,
    3344,  3347,  3352,  3355,  3358,  3364,  3367,  3377,  3380,  3387,
    3390,  3397,  3400,  3407,  3412,  3419,  3425,  3431,  3435,  3442,
    3446,  3453,  3457,  3464,  3467,  3471,  3477,  3480,  3484,  3490,
    3493,  3497,  3502,  3510,  3516,  3522,  3528,  3533,  3536,  3541,
    3550,  3556,  3560,  3566,  3570,  3578,  3585,  3591,  3596,  3600,
    3605,  3610,  3616,  3621,  3627,  3632,  3637,  3641,  3646,  3650,
    3655,  3660,  3665,  3670,  3675,  3681,  3687,  3693,  3698,  3704,
    3710,  3716,  3721,  3727,  3733,  3739,  3744,  3750,  3756,  3765,
    3770,  3775,  3780,  3785,  3790,  3795,  3800,  3807,  3813,  3817,
    3822,  3828,  3834,  3840,  3846,  3854,  3860,  3867,  3873,  3879,
    3885,  3891,  3896,  3901,  3906,  3914,  3919,  3923,  3927,  3934,
    3940,  3946,  3952,  3958,  3963,  3969,  3973,  3979,  3984,  3990,
    3997,  4002,  4008,  4013,  4017,  4023,  4028,  4034,  4039,  4045,
    4050,  4056,  4060,  4065,  4071,  4077,  4083,  4089,  4095,  4099,
    4105,  4111,  4117,  4123,  4129,  4135,  4141,  4156,  4162,  4168,
    4184,  4200,  4216,  4232,  4248,  4264,  4270,  4276,  4282,  4288,
    4332,  4371,  4399,  4423,  4429,  4432,  4439,  4445,  4451,  4455,
    4462,  4466,  4473,  4476,  4483,  4487,  4491,  4495,  4502,  4507,
    4511,  4515,  4522,  4525,  4532,  4539,  4542,  4549,  4555,  4561,
    4564,  4571
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
  "B256FROMRADIX", "B256FUNCTION", "B256GETARRAYBASE", "B256GETBRUSHCOLOR",
  "B256GETCLIPBOARDIMAGE", "B256GETCLIPBOARDSTRING", "B256GETCOLOR",
  "B256GETPENWIDTH", "B256GETSETTING", "B256GETSLICE", "B256GLOBAL",
  "B256GOSUB", "B256GOTO", "B256GRAPHHEIGHT", "B256GRAPHSIZE",
  "B256GRAPHVISIBLE", "B256GRAPHTOOLBARVISIBLE", "B256GRAPHWIDTH",
  "B256GREEN", "B256GREY", "B256GTE", "B256HOUR", "B256IF", "B256IN",
  "B256IMAGEAUTOCROP", "B256IMAGECENTERED", "B256IMAGECOPY",
  "B256IMAGECROP", "B256IMAGEDRAW", "B256IMAGEFLIP", "B256IMAGEHEIGHT",
  "B256IMAGELOAD", "B256IMAGENEW", "B256IMAGEPIXEL", "B256IMAGERESIZE",
  "B256IMAGEROTATE", "B256IMAGESETPIXEL", "B256IMAGESMOOTH",
  "B256IMAGETRANSFORMED", "B256IMAGETYPE_BMP", "B256IMAGETYPE_JPG",
  "B256IMAGETYPE_PNG", "B256IMAGEWIDTH", "B256IMGLOAD", "B256IMGSAVE",
  "B256IMPLODE", "B256INPUT", "B256INPUTFLOAT", "B256INPUTINT",
  "B256INPUTSTRING", "B256INSTR", "B256INSTRX", "B256INTDIV",
  "B256ISNUMERIC", "B256KEY", "B256KEYPRESSED", "B256KILL",
  "B256LASTERROR", "B256LASTERROREXTRA", "B256LASTERRORLINE",
  "B256LASTERRORMESSAGE", "B256LEFT", "B256LENGTH", "B256LET", "B256LINE",
  "B256LJUST", "B256LOG", "B256LOGTEN", "B256LOWER", "B256LTE",
  "B256LTRIM", "B256MAINTOOLBARVISIBLE", "B256MAP", "B256MD5", "B256MID",
  "B256MIDX", "B256MINUTE", "B256MOD", "B256MONTH", "B256MOUSEB",
  "B256MOUSEBUTTON_CENTER", "B256MOUSEBUTTON_DOUBLECLICK",
  "B256MOUSEBUTTON_LEFT", "B256MOUSEBUTTON_NONE", "B256MOUSEBUTTON_RIGHT",
  "B256MOUSEX", "B256MOUSEY", "B256MSEC", "B256MULEQUAL", "B256NE",
  "B256NETADDRESS", "B256NETCLOSE", "B256NETCONNECT", "B256NETDATA",
  "B256NETLISTEN", "B256NETREAD", "B256NETWRITE", "B256NEXT", "B256NOT",
  "B256OFFERROR", "B256ONERROR", "B256OPEN", "B256OPENB",
  "B256OPENFILEDIALOG", "B256OPENSERIAL", "B256OR", "B256ORANGE",
  "B256OSTYPE", "B256OSTYPE_ANDROID", "B256OSTYPE_LINUX",
  "B256OSTYPE_MACINTOSH", "B256OSTYPE_WINDOWS", "B256OUTPUTVISIBLE",
  "B256OUTPUTTOOLBARVISIBLE", "B256PAUSE", "B256PENWIDTH", "B256PI",
  "B256PIE", "B256PIXEL", "B256PLOT", "B256POLY", "B256PORTIN",
  "B256PORTOUT", "B256PRINT", "B256PRINTERCANCEL", "B256PRINTEROFF",
  "B256PRINTERON", "B256PRINTERPAGE", "B256PROMPT", "B256PURPLE",
  "B256PUTSLICE", "B256RADIANS", "B256RAND", "B256READ", "B256READBYTE",
  "B256READLINE", "B256RECT", "B256RED", "B256REDIM", "B256REF",
  "B256REFRESH", "B256REGEXMINIMAL", "B256REM", "B256REPLACE",
  "B256REPLACEX", "B256SAVEFILEDIALOG", "B256RESET", "B256RETURN",
  "B256RGB", "B256RIGHT", "B256RJUST", "B256ROUND", "B256RTRIM", "B256SAY",
  "B256SECOND", "B256SEED", "B256SEEK", "B256SEMICOLON",
  "B256SEMICOLONEQUAL", "B256SERIALIZE", "B256SETCLIPBOARDIMAGE",
  "B256SETCLIPBOARDSTRING", "B256SETCOLOR", "B256SETGRAPH",
  "B256SETSETTING", "B256SIN", "B256SIZE", "B256SLICE_ALL",
  "B256SLICE_PAINT", "B256SLICE_SPRITE", "B256SOUND", "B256SOUNDENVELOPE",
  "B256SOUNDFADE", "B256SOUNDHARMONICS", "B256SOUNDID", "B256SOUNDLENGTH",
  "B256SOUNDLOAD", "B256SOUNDLOADRAW", "B256SOUNDLOOP", "B256SOUNDPAUSE",
  "B256SOUNDPLAY", "B256SOUNDPLAYER", "B256SOUNDPLAYEROFF",
  "B256SOUNDPOSITION", "B256SOUNDSAMPLERATE", "B256SOUNDSEEK",
  "B256SOUNDSTATE", "B256SOUNDSTOP", "B256SOUNDSYSTEM", "B256SOUNDVOLUME",
  "B256SOUNDWAIT", "B256SOUNDWAVEFORM", "B256SPRITECOLLIDE",
  "B256SPRITEDIM", "B256SPRITEH", "B256SPRITEHIDE", "B256SPRITELOAD",
  "B256SPRITEMOVE", "B256SPRITEO", "B256SPRITEPLACE", "B256SPRITEPOLY",
  "B256SPRITER", "B256SPRITES", "B256SPRITESHOW", "B256SPRITESLICE",
  "B256SPRITEV", "B256SPRITEW", "B256SPRITEX", "B256SPRITEY", "B256SQR",
  "B256STAMP", "B256STEP", "B256SUB1", "B256SUBEQUAL", "B256SUBROUTINE",
  "B256SYSTEM", "B256TAN", "B256TEXT", "B256TEXTHEIGHT", "B256TEXTWIDTH",
  "B256THEN", "B256THROWERROR", "B256TO", "B256TOBINARY", "B256TOFLOAT",
  "B256TOHEX", "B256TOINT", "B256TOOCTAL", "B256TORADIX", "B256TOSTRING",
  "B256TRIM", "B256TRY", "B256TYPEOF", "B256TYPE_ARRAY", "B256TYPE_FLOAT",
  "B256TYPE_INT", "B256TYPE_MAP", "B256TYPE_REF", "B256TYPE_STRING",
  "B256TYPE_UNASSIGNED", "B256UNASSIGN", "B256UNLOAD", "B256UNSERIALIZE",
  "B256UNTIL", "B256UPPER", "B256VARIABLEWATCH", "B256VERSION",
  "B256VOLUME", "B256WARNING_ARRAYELEMENT", "B256WARNING_BOOLEANCONV",
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
  "netwritestmt", "netclosestmt", "killstmt", "setclipboardimagestmt",
  "setclipboardstringstmt", "setsettingstmt", "portoutstmt", "imgsavestmt",
  "editvisiblestmt", "graphvisiblestmt", "outputvisiblestmt",
  "maintoolbarvisiblestmt", "graphtoolbarvisiblestmt",
  "outputtoolbarvisiblestmt", "globalstmt", "penwidthstmt", "alertstmt",
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
     745,   746,   747,   748,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,   760,   761,    44,    60,    62,
      61,    45,    43,    42,    47,   762,    94,    91,    93,   123,
     125,    40,    41,    10,    58,    63,    37
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   527,   528,   528,   529,   530,   530,   530,   530,   530,
     530,   530,   530,   531,   532,   532,   533,   534,   535,   535,
     536,   536,   537,   537,   537,   538,   538,   539,   539,   539,
     540,   540,   541,   542,   542,   543,   543,   544,   545,   545,
     545,   546,   547,   547,   548,   549,   549,   550,   550,   551,
     551,   552,   552,   553,   553,   554,   554,   555,   555,   556,
     556,   557,   557,   558,   558,   559,   559,   560,   560,   561,
     561,   562,   562,   563,   563,   563,   563,   563,   563,   563,
     563,   564,   564,   564,   564,   564,   565,   565,   566,   566,
     566,   566,   566,   566,   566,   566,   566,   566,   566,   566,
     566,   566,   566,   566,   566,   566,   566,   566,   566,   566,
     566,   566,   566,   566,   566,   566,   566,   566,   566,   566,
     566,   566,   566,   566,   566,   566,   566,   566,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   567,   567,   567,   567,   567,
     567,   567,   567,   567,   567,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     570,   570,   570,   570,   570,   570,   570,   570,   570,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   572,   574,   573,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   584,   585,   586,
     588,   587,   590,   589,   591,   591,   591,   591,   591,   591,
     592,   593,   593,   593,   593,   593,   593,   593,   593,   594,
     594,   594,   594,   594,   594,   595,   596,   597,   597,   597,
     598,   599,   599,   600,   601,   602,   602,   602,   602,   602,
     602,   602,   602,   602,   603,   604,   604,   604,   604,   604,
     604,   604,   604,   604,   605,   605,   606,   606,   607,   607,
     608,   609,   610,   610,   611,   612,   612,   612,   613,   613,
     614,   614,   614,   615,   615,   616,   616,   616,   617,   617,
     618,   618,   619,   619,   620,   620,   621,   621,   622,   623,
     623,   623,   624,   624,   624,   625,   625,   626,   626,   627,
     627,   628,   628,   629,   630,   631,   632,   633,   633,   634,
     634,   635,   635,   636,   636,   636,   637,   637,   637,   638,
     638,   638,   638,   639,   640,   641,   642,   643,   643,   643,
     644,   644,   644,   644,   644,   644,   644,   644,   644,   644,
     645,   645,   646,   646,   647,   647,   648,   648,   649,   649,
     650,   651,   651,   652,   652,   652,   652,   652,   652,   652,
     652,   652,   652,   652,   652,   652,   652,   652,   652,   653,
     653,   653,   653,   653,   653,   653,   653,   654,   655,   655,
     656,   657,   658,   659,   660,   660,   660,   661,   662,   663,
     664,   665,   665,   665,   665,   666,   666,   666,   666,   667,
     668,   669,   670,   671,   671,   672,   672,   673,   673,   674,
     674,   674,   675,   675,   675,   676,   676,   677,   677,   678,
     678,   679,   679,   680,   681,   682,   683,   684,   685,   685,
     686,   687,   688,   689,   690,   691,   692,   693,   694,   695,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   708,   709,   710,   710,   711,   712,   713,   713,
     714,   714,   715,   715,   716,   716,   716,   716,   717,   717,
     717,   717,   718,   718,   719,   720,   720,   721,   722,   723,
     723,   724
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
       3,     3,     2,     2,     2,     2,     2,     2,     2,     2,
       4,     4,     4,     4,     6,     8,    10,     6,     8,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     2,     2,     2,     4,     4,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     4,     2,
       4,     2,     4,     2,     2,     2,     2,     2,     2,     6,
       8,    10,     2,     2,     2,     8,     6,     4,     4,     4,
       4,     4,     4,     4,     4,     2,     4,     6,     4,     6,
       8,     4,     6,     8,     4,     6,     8,     2,     2,     2,
       4,     4,     6,     8,     6,     2,     2,     4,     6,     2,
       6,     2,     4,     2,     2,     4,     2,     2,     2,     4,
       6,     4,     4,     4,     6,     1,     1,     1,     2,     2,
       2,     4,     4,     4,     2,     4,     2,     4,     2,     4,
       2,     2,     4,     4,     4,     4,     4,     4,     4,     2,
       2,     2,     2,     1,     4,     4,     4,     4,     8,     6,
       8,     6,     6,     2,     4,     2,     4,     2,     4,     6,
       8,     2,     2,     2,     4,     2,     4,     2,     2,     6,
       4,     2,     8,    10,     8,     4,     4,     4,     4,     4,
       6,     4,     4,     4,     6,     4,     4,     4,     4,     4,
       2,     2,     2,     4,     4,     4,     8,     6,     4,    12,
      10,     4,     2,     4,     4,     4,     4,     4,     4,     4,
       1,     1,     1,     4,     2,     2,     2,     2,     2,     1,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     2,     2,     3,     1,
       0,     3,     0,     3,     2,     2,     2,     1,     1,     1,
       2,     2,     4,     3,     5,     3,     5,     4,     4,     2,
       4,     3,     5,     3,     5,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     3,     3,
       3,     3,     3,     3,     6,     8,     4,     7,     2,     1,
       2,     2,     4,     5,     2,     2,     4,     5,     2,     2,
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
       3,     2,     4,     2,     3,     2,     4,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     3,
       3,     1,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      12,     0,     0,     0,   645,     0,   646,   648,     0,     0,
       0,    52,    52,    52,    52,   899,   900,   901,    52,    52,
       0,     0,     0,     0,   649,     0,     0,   650,    52,   651,
     911,   652,   912,   653,   654,   902,   903,   904,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,   719,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      52,    52,    52,    52,     0,     0,     0,    52,     0,    11,
      52,    52,     0,     0,     0,     0,     0,     0,    52,     0,
       0,    52,     0,    52,     0,    52,    52,    52,     0,    52,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   659,     0,     0,
     660,     0,     0,    52,    52,     0,    52,    52,   662,     0,
       0,     0,    25,    13,     0,    52,     0,     3,     9,     7,
      27,     0,     0,    15,   502,   504,   505,   522,   526,   527,
     529,   532,   533,   548,   549,   550,     0,   630,   633,   641,
     565,   568,   521,   592,   579,   629,   509,   537,   545,   593,
     530,   669,   668,   667,   539,   540,   573,   544,   543,   503,
     574,   575,   596,   512,   603,   606,   604,   605,   607,   608,
     609,   610,   611,   612,   613,   614,   615,   616,   582,   566,
     508,   525,   500,   507,   581,   591,   628,   538,   597,   627,
     635,   583,   625,   576,   644,   643,   642,   511,   595,   598,
     599,   563,   589,   636,   637,   638,   639,   640,   590,   561,
     617,   619,   624,   622,   621,   620,   618,   623,   510,   506,
     520,   517,   518,   519,   516,   571,   570,   572,   569,   564,
     600,   601,   602,   584,   562,   524,   546,   577,   567,   547,
     578,   542,   580,   499,   513,   514,   515,   534,   535,   536,
     585,   586,   587,   588,   541,   626,   528,   531,   594,   631,
     634,   553,   551,   557,   556,   554,   552,   560,   558,   555,
     559,   632,   523,   501,     0,     0,     0,     0,     0,     0,
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
       0,    52,    52,    52,    52,    52,    52,     0,     0,    52,
      52,    52,    52,    52,    52,     0,     0,     0,     0,    52,
      52,    52,     0,     0,     0,     0,     0,    52,    52,    52,
      52,    52,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,     0,     0,    52,
      52,    52,    52,    52,    52,    52,     0,     0,     0,    52,
       0,    52,    52,    52,    52,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     0,     0,    52,    52,    52,
      52,    52,    52,     0,     0,     0,    52,    52,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    52,    52,    52,    52,    52,    52,     0,
       0,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,     0,   255,   256,   423,   396,   395,   397,
       0,     0,     0,     0,    27,   490,    80,   491,   492,   898,
      73,    74,    75,    76,    77,    78,    79,     0,   767,   768,
       0,   941,     0,     0,     0,   862,   769,   770,     0,   765,
       0,     0,   688,   689,     0,   861,   806,   807,   687,   865,
     866,     0,   872,   874,   873,     0,   868,   867,   864,   863,
       0,   870,   871,   869,    27,     0,   671,   890,     0,   766,
       0,   694,   690,     0,   781,   780,   779,   782,     0,     0,
       0,     0,    27,    32,    35,   896,   721,   720,   691,   692,
       0,   891,   894,     0,   919,   918,     0,   931,   930,   929,
     928,     0,     0,   917,     0,   927,   926,   925,   924,   936,
     935,   921,   920,   934,     0,     0,   923,   922,     0,   937,
       0,   933,   932,     0,     0,   844,   845,   846,     0,   889,
     888,     0,    27,    80,   815,   813,     0,    27,    80,   827,
     825,    27,    80,   823,   821,    27,    80,   819,   817,   883,
     666,   665,   664,   764,   893,   670,   881,   882,   877,   878,
     876,   875,   880,   879,   718,   724,   725,   791,   790,   793,
     792,     0,   794,   795,   796,   797,   798,   799,     0,   892,
     895,   685,   897,   771,   772,   763,   786,   887,     0,   831,
     829,    42,   905,   906,   907,   908,   843,     0,   773,   774,
     775,     0,     0,   679,   693,   913,   808,   809,   728,   729,
     783,   810,   812,   811,   884,   885,   731,   732,   730,   940,
     939,   886,   733,   734,     0,   749,   750,   751,   756,   755,
     752,   753,   754,   761,   762,   739,   738,   735,   736,   737,
     741,   740,   757,   758,   743,   742,   748,   759,   760,   745,
     744,   747,   746,   847,   859,   848,   855,   856,   857,   858,
     851,   852,   853,   854,   850,   860,   849,   789,   788,   787,
       0,   784,     0,   776,   777,   778,     0,   686,   914,   915,
     938,     0,   916,   785,   837,   838,   839,   840,   841,   842,
       0,   801,   800,   805,   804,   803,   802,     0,     0,     0,
       0,   835,   833,    42,     1,     4,    12,    10,     5,     8,
     706,     0,     0,     0,     0,     0,   707,     0,     0,    28,
      30,     0,     0,   696,     0,     0,     0,     0,     0,   697,
       0,     0,     0,   656,   657,     0,     0,     0,   288,   284,
       0,     0,     0,     0,   270,    88,    89,     0,   316,    90,
      91,     0,     0,    92,   338,   336,   337,     0,     0,     0,
       0,   437,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   321,     0,     0,     0,     0,   355,     0,     0,     0,
       0,   451,   128,   129,   130,   131,   132,   133,   134,   135,
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
     236,   237,   238,   239,     0,     0,   494,   495,   496,     0,
     383,     0,   384,   386,   387,     0,     0,     0,     0,   422,
     343,   447,   448,   342,   344,     0,     0,   497,   498,     0,
     326,   325,   102,   103,   322,     0,   482,     0,     0,     0,
       0,   470,   471,   472,     0,     0,     0,     0,     0,     0,
     331,     0,   329,   367,   442,   368,   441,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   323,   320,   335,
     104,   105,   106,   107,   108,   333,   334,   376,   445,     0,
     369,     0,   443,   274,     0,   109,   375,   110,   111,   112,
     113,   315,     0,     0,     0,   114,     0,   314,     0,   433,
       0,   381,     0,   435,   115,     0,     0,     0,     0,     0,
       0,     0,     0,   324,     0,     0,     0,   327,   116,   117,
     118,   404,     0,   410,     0,     0,     0,     0,   406,   411,
       0,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   289,   285,     0,     0,   379,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   120,   121,
     122,   123,   124,   125,     0,     0,   388,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   398,   399,   400,   126,   319,   127,     0,   259,
     257,   258,     0,     0,    39,    38,     0,    42,    27,   490,
      80,     0,   286,   287,     0,     0,    28,   419,   420,   421,
     282,   283,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,   647,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,   675,   673,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   909,     0,     0,     0,   655,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   830,     0,     0,     0,   683,   681,     0,   910,
       0,     0,   661,   663,    26,    29,    31,     0,   834,     2,
       6,   708,   712,   711,   710,   713,   709,    16,     0,   704,
     705,   698,   702,   701,   700,   703,   699,   695,    14,   658,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    44,
       0,    81,     0,     0,     0,    87,     0,    47,    49,     0,
      21,    19,    84,   271,   267,   268,   269,   279,   281,   264,
     280,   261,   276,   272,    83,   273,   277,   278,   275,   260,
      82,    85,   265,   266,   262,    64,    66,     0,   722,     0,
      60,     0,    54,    53,    53,   678,   677,     0,     0,   672,
      62,     0,    53,     0,   716,     0,    33,     0,    36,     0,
       0,     0,    70,    72,     0,     0,    56,    58,    57,    55,
     726,     0,    68,    53,   832,    43,    42,     0,     0,     0,
     680,     0,   836,     0,    24,   313,   305,   293,   304,   417,
     418,   306,   317,   299,   424,     0,   389,   302,     0,     0,
       0,   361,     0,   358,     0,   364,     0,   356,     0,   438,
     465,   307,   450,   318,   312,   300,   385,   391,   392,   393,
       0,     0,     0,     0,   481,   413,   478,     0,   414,   412,
     457,   458,   456,     0,     0,   466,   332,   330,     0,   292,
     483,   484,   309,   310,   427,   467,   446,     0,     0,   370,
     444,   488,     0,   371,     0,   459,   308,   434,   382,   436,
       0,     0,   489,     0,     0,   485,   486,   416,   415,   468,
     455,   301,   328,   409,   474,   473,   475,   403,   402,   405,
     407,     0,   349,   354,   352,   353,   351,   350,   347,   348,
     311,   303,     0,     0,   377,   461,   291,   462,   290,   463,
       0,   425,   469,   401,   493,   426,   487,    41,    40,     0,
       0,     0,    17,     0,    86,     0,     0,     0,   723,     0,
     676,   674,     0,     0,     0,     0,    34,     0,     0,     0,
       0,   727,     0,     0,   684,   682,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    45,    20,    18,
       0,    48,     0,    59,     0,    59,   714,     0,    59,     0,
       0,    59,    59,     0,    59,    22,     0,   390,     0,   372,
     374,     0,   362,     0,   359,     0,   365,   357,     0,   439,
     394,   449,     0,     0,     0,   477,     0,   294,     0,   297,
     431,     0,     0,   429,   339,   460,     0,     0,     0,   432,
       0,   346,   380,   378,   464,     0,    50,     0,     0,     0,
     717,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46,     0,     0,    61,   715,    61,     0,     0,
      61,    61,   373,   363,   360,   366,   440,     0,   476,     0,
     295,   298,   428,   430,     0,   452,   454,     0,   340,   345,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    63,
       0,    63,     0,   480,   296,   453,   341,     0,     0,     0,
       0,    65,     0,    65,   479,     0,     0,     0,    67,     0,
       0,     0,    69,     0,    71
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   156,   936,   157,   158,   159,  1326,  1327,  1328,  1329,
     950,   644,   645,   646,   714,  1385,   715,   647,  1313,  1314,
    1315,   648,  1316,  1576,  1577,   672,  1366,   764,   765,  1362,
    1376,  1355,  1356,   807,   751,   752,   720,   650,   651,   652,
     653,   654,   655,   656,   163,   164,   165,   664,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   911,   179,   920,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1467
static const int yypact[] =
{
   20991,  8762,  9272,  8762, -1467,  -430, -1467, -1467,  8762,  9272,
    9782, 10292,  -485, 10292,  -485, -1467, -1467, -1467, 10292, 10802,
   11312, 11312, 11822,  -429, -1467,  8762, 12332, -1467,  -485, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,  -485, 12842,
    -430,  -430,  -430,  -360,  -430,  -430, 11822,  8762,  8762,  8762,
   11312, 13352, 13862, 13352, 11822, 11822, 11312, 14372,  8762, 14882,
   15392, 11312, 15902, 15902, 15902, 15902,  8762,  -429, 12332,  8762,
    -360, 10292, 11822, 11312, 11312,  -430,  -485,  -430, 11312, 11312,
   16412,  8762,  8762,  8762,  8762,  9272, 11312,  8762, 11312, 16922,
    -485,  -485,  -485,  -485,  9782, 17432,  -429,  -485,  8762, -1467,
   10292, 10292,  8762,  8762, 11312,  8762,  8762, 11822, 10292,  9782,
   11312, 17942, 14372, 10802, 11312, 10292, 10802, 10292, 11312, 10292,
    8762, 11312, 10292, 11312,  8762,  8762, 11312, 13352, 13352, 11312,
    8762, 13862, 15392,  -430,  8762, 18452,  8762, -1467,  -429,  8762,
   -1467,  -360,  8762,  -485, 10292,  8762,  -485,  -485, -1467, 11312,
   11312, 11312, -1467, -1467,  -429, 18962,    40,  -452, 21491,  -265,
     187,  -180,   169,  -426, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, 21959, -1467, -1467, 21959, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467,  -382,  -381,  -359,  -361,  -356,  -355,
    -351,  8762,  -485,  -485,  -350,  -485,  -485,  -348,  -345,  -485,
    -485,  -485,  -485,  -342,  -341,  -340,  -338,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -335,  -334,
    -333,  -330,  -326,  -324,  -323,  -322,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -321,  -320,
   11822, 11312,  -319,  -485,  -318,  -485,  -485,  -317,  -316,  -315,
    -314,  -485,  -485,  -485,  -485,  -485,  -485,  -312, 19472,  -485,
    -485,  -485,  -485,  -485,  -310,  -308,  -307,  -306,  -304,  -485,
    -485,  -485,  -303,  -302,  -301,  -299,  -295,  -294,  -292,  -485,
    -485,  -485,  -485,  -290,  -288,  -286,  -285,  -282,  -281,  -280,
    -278,  -276,  -273,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -272,  -271,  8762,  -269,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -268,  -267,  -266,  -485,
    -264,  -485,  -263,  -262,  -261,  -485,  -260,  -257,  -256,  -255,
    -254,  -252,  -248,  -246,  -485,  -245,  -244,  -243,  -485,  -485,
    -485,  -485,  -241,  -236,  -234,  -233,  -232,  -485,  -231,  -229,
    -228,  -227,  -226,  -224,  -223,  -221,  -220,  -219,  -218,  -359,
    -215,  -214,  -212,  -211,  -210,  -209,  -208,  -206,  -205,  -204,
    -203,  -202,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -199,
    -198,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -197, -1467, -1467, -1467, -1467, -1467, -1467,
    8762,  -457, 19982,  8762,    11, -1467,    18, -1467, -1467, 36382,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467,  9272, -1467, -1467,
   24828, 36382,  -430,  -195,  8762, 36382, -1467, -1467,  9782, -1467,
   24861,  5192, -1467, 36382,  -194, -1467, -1467, 36382, -1467, -1467,
   36382,  5702, -1467, -1467, 24911, 11312, -1467, 24911, -1467, 24911,
   11822, -1467, -1467, 24974,  -190,  6722,    49, 36382, 12332, -1467,
   25120, -1467, -1467, 12842, -1467, -1467, -1467, 25146,  -181,  -237,
    -189,  -360,  -186, -1467,  -216, -1467, -1467, -1467, -1467, -1467,
   24974, 36382, 36382, 25168, -1467, 24911, 13352, -1467, -1467, -1467,
   -1467, 25201, 13862, -1467, 25264, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, 24911, 14372, -1467, -1467, 25428, 36382,
   14882, -1467, -1467, 25458, 15392, -1467, -1467, -1467, 25491, -1467,
   24911, 15902,    90,   153, -1467, -1467, 25518,   137,   159, -1467,
   -1467,   151,   163, -1467, -1467,   189,   172, -1467, -1467, 36382,
   -1467, -1467, -1467, -1467, 36382, -1467, -1467, 36382, -1467, -1467,
   -1467, 24911, -1467, 24911, -1467, -1467,  -188, -1467, 24911, -1467,
   24911, 16412, -1467, -1467, -1467, -1467, -1467, -1467, 25574, 36382,
   36382, 36382, 36382, -1467, -1467, -1467, 36382, -1467,  5192, -1467,
   -1467, 25601, -1467, -1467, -1467, -1467, -1467, 17432, -1467, -1467,
   -1467, 25685, 11312,   127, -1467, 36382, -1467, 36382, -1467, 36382,
   36382, 36382, -1467, 24911, 36382, 36382, -1467, -1467, 24974, -1467,
   36382, -1467, -1467, 24911,  6212, -1467, -1467, 25120, -1467, -1467,
   -1467, -1467, 24911, -1467, 24911, -1467, 36382, -1467, -1467, 24911,
   -1467, 36382, -1467, 24911, -1467, 36382, 36382, -1467, 24911, -1467,
   36382, -1467, 24911, 36382, 36382, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, 36382, -1467, -1467, -1467, -1467,
    -189, 36382, 18452, -1467, -1467, -1467, 25828, 36382, -1467, -1467,
   36382,  8762, -1467, 36382, -1467, -1467, 36382, 36382, -1467, -1467,
    8762, -1467, 24911, -1467, 24911, -1467, 24911,  -483,  -187,  -178,
    5192, -1467, -1467, 25855, -1467, -1467, 20991, -1467,   -19, -1467,
   -1467,  8762,  8762,  8762,  8762,  8762, -1467,  8762,  7232, -1467,
   -1467,  8762,  8762, -1467,  8762,  8762,  8762,  8762,  8762, -1467,
    8762,  8762, 21959,   262, -1467,  8762,  8762,  -359,  -172, -1467,
    8762,  8762,  -429,  8762,  -481, -1467, -1467,  5192, -1467, -1467,
   -1467,  8762,  8762, -1467, -1467, -1467, -1467,  8762,  8762,  8762,
    8762, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,  8762,  8762,  8762,  5192, -1467,  8762,  8762,  8762,
    5192, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467,  8762,  8762, -1467, -1467, -1467,  8762,
   -1467,  5192, -1467, -1467, -1467,  8762,  8762,  8762,  8762, -1467,
   -1467, -1467, -1467, -1467, -1467,  8762, 19472, -1467, -1467, 25895,
   -1467, -1467, -1467, -1467, -1467,  5192, -1467,  8762,  8762,  8762,
    9782, -1467, -1467, -1467,  8762, 11822,  8762,  8762,  8762,  5192,
   -1467,  5192, -1467, -1467, -1467, -1467, -1467,  8762,  8762, 11822,
    8762,  8762,  8762,  8762,  8762,  8762,  8762, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,  5192,
   -1467,  5192, -1467,  1792,  9782, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,  8762,  8762,  8762, -1467,  8762, -1467,  5192, -1467,
    5192, -1467,  5192, -1467, -1467,  8762,  8762,  9782,  8762,  8762,
   11822, 11312,  8762, -1467,  8762,  8762,  5192, -1467, -1467, -1467,
   -1467, -1467,  5192, -1467, 11312,  8762, 11312,  5192, -1467, -1467,
    5192, -1467,  8762,  8762,  8762,  8762,  8762,  8762,  8762,  8762,
    8762,  8762,  -172, -1467,  8762,  5192, -1467,  8762,  8762,  8762,
    8762,  8762,  8762,  8762,  8762,  8762,  8762, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467,  8762,  8762, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, 11312,  -481,
   -1467, -1467, 19982,  -171,  -159, -1467,  -168, 25939,    17,  -187,
      23, 26002, -1467, -1467,  3143,  3662,  -164,  -186, -1467, -1467,
   -1467, -1467,  8762,  8762,  8762,  8762,  8762,  8762,  8762,  8762,
    8762,  8762,  8762,  8762,  8762,  8762,  8762,  8762,  8762,  8762,
    8762,  8762,  8762,  8762, 20492,  -167,  -165,   784,  8762,  -163,
    4172, 36382,  -162,  1814,  8762, -1467,  -161,  1898,  8762,  1921,
    8762,  8762,  8762,   155,  1971,  8762,  -158,  2049,  8762,  2230,
    8762,  8762,   -20,  -151,  -468, -1467,  -448,  -153,  -360, -1467,
   21152,  8762, 22120,  8762, 22227,  8762,  -156,  -152, 22301,  8762,
   22462,  8762,  -146,  -143, 22534,  -359,  4682,  -142, 22558,  8762,
    -141, 22592,  8762,  8762, 22630,  8762,   161, 22706,  8762, -1467,
   22884,  8762, 36382, 36382, -1467, -1467, -1467,  -140,  8762, -1467,
   -1467, 36382, 36382, 36382, 36382, 36382, 36382, -1467, 22939, 36382,
   36382, 36382, 36382, 36382, 36382, 36382, 36382, 36382, -1467, -1467,
   26037, 26149,  -370,  8762, 26197, 26248,  -138,  -137, 26376, 26406,
   26550, 26572, 22997, 26633, 26666, 26714, 23111, 23166, 23256, 23278,
   23320, 26807, 26829, 26923, 26958, 27089, 27119, 27240, 27264, 27354,
   27384, 27497, 27644, 23401,  8762, 23423, 27666, 27692, 27787,  -136,
   27809,  -134,  -130, 23568, 27835, 27951, 28044, 28068, 28094, 28116,
   28209,  -129,  -128, 28375, 28418, 28466, 28488, 28522, 28629, 28781,
   28803, 28928,  -124, 28950, 29039, 23683, 29094, 29207, 29249, 29330,
   29352, 29375,  -123, 29497, 29654,  -122,  -121,  -120, 23713, 29676,
   29776, 29798, 29887, 30034,  -118, 23803, 30057,  -116, 23858, 30097,
   30120, 30196, 30356, 30429, 30455, 30479, 30503, 30736, 30794, 30820,
   30842, 30865, 30887, 23880, 30909, 31144, 31166, 31200, 31245, 31267,
   31494, 31524, 31558, 31580, 31603,  -115,  -112, -1467, 19982, -1467,
    7742, -1467,  -114,  -474,  -111, -1467,  -113,   -95, 36382,  -110,
   -1467, -1467,   226,  1792,   226,  -230,  -230,   204, 36641,  -481,
     204,  -481,   204, 23230, 21108, 23230,   204,   204,   204,  -139,
    -139,  -481,  -481,  -157,  -481, -1467, -1467, 31625, -1467,  -109,
   -1467, 31882, -1467, 36382, 31882, 36382, 36382,  8762,  8762, 36382,
   -1467, 31904, 31927, 31965, -1467,  -430, -1467,  -108, -1467, 32003,
   32212, 31927, -1467, -1467, 32247, 32285, -1467, -1467,  -172, -1467,
   -1467,  -106, -1467, 32309, -1467, -1467, 32336, 32594,  8762,  8762,
   36382, 32617, -1467,  8762,  -100, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,  8762, -1467, -1467,  8762,  8762,
    8762, -1467,  8762, -1467,  8762, -1467,  8762, -1467,  8762, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
    8762,  8762, 32644,  8762, -1467, -1467, -1467,  8762, -1467, -1467,
   -1467, -1467, -1467,  8762,  8762, -1467, -1467, -1467,  8762, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467,  8762,  8762, -1467,
   -1467, -1467,  8762, -1467,  8762, -1467, -1467, -1467, -1467, -1467,
    8762,  8762, -1467,  8762,  8762, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467,  8762, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,  8762,  8762, -1467, -1467, -1467, -1467, -1467, -1467,
    8762, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,  8762,
    -135,   -99, -1467,  -430, -1467,  8252,   -98,  8762, -1467,  8762,
   36382, 36382,  8762,  8762,  8762,   176, -1467,  8762,  8762,  8762,
    8762, -1467,  8762,  8762, 36382, 36382,  8762, 32667,  8762, 32689,
   23973, 32756, 24036, 24115, 24137, 32925, 24230, 33002, 33037,  8762,
   33067, 24263, 24285, 24427, 33294, 33327, 24457, 33349, 33375, 33397,
   33436, 33469, 33696, 24520, 33718, 33751, 33777, 33807, -1467, -1467,
     -97, -1467, 34042, 36382, 34069, 34069, 34092,   -20, 34114, 34136,
   34159, 34235, 34444, 34114, 34042, -1467, 34468, -1467,  8762, -1467,
   -1467,  8762, -1467,  8762, -1467,  8762, -1467, -1467,  8762, -1467,
   -1467, -1467, 34235,  8762,  8762, -1467,  8762, -1467,  8762, -1467,
   -1467,  8762,  8762, -1467, -1467, -1467,  8762,  8762,  8762, -1467,
    8762, -1467, -1467, -1467, -1467,  8762, -1467,  8762,  8762,  8762,
   -1467,  8762,  8762,  8762,  8762,  8762, -1467, 34494, 34552, 34703,
   34779, 34809, 34842, 34865, 24570, 34900, 35127, 35157, 24684, 35213,
   24747, 35235, -1467, 36618, 35258, 36382, 36382, 35258, 35322, 35520,
   35322, 35555, -1467, -1467, -1467, -1467, -1467,  8762, -1467,  8762,
   -1467, -1467, -1467, -1467,  8762, -1467, -1467,  8762, -1467, -1467,
    8762,  8762,  8762,  8762, 24805, 35582, 35666, 35696, 35847, 36382,
   35931, 35958,  8762, -1467, -1467, -1467, -1467,  8762,  8762,  8762,
   35991, 36382, 36101, 36240, -1467,  8762,  8762, 36270, 36382,  8762,
   36310,  8762, 36360,  8762, 36382
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1467,  -514, -1467, -1467, -1467,  -147,  -154,  -903, -1467, -1467,
   -1467,  1381,   167,  1430, -1467,  -472,   -67, -1467, -1142, -1467,
       0, -1467, -1466, -1224, -1467,  2501,   317,   -50,   -33,   435,
     717,   219,   500,  -369,  -313,  -311,    -1, -1467, -1467, -1467,
   -1467, -1467, -1467, -1378, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,   367,   369,   377, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -829
static const yytype_int16 yytable[] =
{
     649,   660,   661,   785,  1624,  1382,   949,   665,   660,   670,
     673,   938,   677,   769,   773,   777,  1322,   680,   684,   687,
     689,   693,  1322,  1330,   697,   700,   963,   951,  1330,   964,
     770,   774,   778,  1781,   948,  1353,   674,   152,   707,  1424,
     934,  1339,  1310,  1311,  1782,  1354,   721,   722,   723,   725,
     731,   734,   731,   711,  1626,   744,   748,   749,   753,   758,
     760,   766,   766,   766,   766,   779,  1341,   700,   784,  1387,
     787,   935,   791,   793,  1424,   152,   152,   798,   800,   808,
     809,   810,   811,   812,   660,   744,   816,   744,   821,   819,
    -814,   662,   154,   670,   831,  1322,   939,   835,   962,   837,
     839,   840,   841,   843,   844,   845,   848,   850,   670,   853,
     857,   748,   862,   864,   866,   869,   871,   873,   875,   876,
     878,   880,   882,   883,   884,   744,   731,   731,   744,   895,
     734,   758,  1339,   901,   906,   907,  1609,  -826,   910,   965,
     966,   913,  1322,   916,   917,   152,   152,  1453,   922,   924,
     926,  -822,  1424,  -816,   933,   931,  1322,  1341,  1330,  -828,
     970,   711,   967,  -824,  1330,   971,   972,   161,  1330,  -814,
     973,   977,  -820,   981,   953,   954,   982,  1330,   955,   987,
     988,   989,  1641,   990,   470,   471,  1002,  1003,  1004,  -818,
     695,  1005,   940,   941,  1322,  1007,   942,  1008,  1009,  1010,
    1124,  1125,  1129,  1131,  1135,  1136,  1137,  1138,   488,  1145,
     713,  1155,  1332,  1157,  1158,  1159,  -826,  1160,  1164,  1165,
    1166,   658,  1167,  1334,  1335,  1336,  1168,  1169,   666,  1171,
    -822,  1177,  -816,  1178,   161,  1179,  1180,   713,  -828,  1181,
    1182,  1183,  -824,  1184,   956,  1185,  1335,  1336,  1186,  1199,
    1201,  -820,  1204,  1212,  1213,  1214,  1375,  1216,  1218,  1220,
    1222,  1225,   943,   832,  1226,  1227,  1228,  1229,  -818,  1230,
     729,   733,   737,  1231,  1383,  1232,  1234,  1235,  1236,   757,
    1242,  1349,  1350,  1351,  1352,  1244,  1353,  1245,  1246,  1247,
    1250,  1388,  1252,  1253,  1254,  1255,  1354,  1256,  1257,   805,
    1258,  1259,  1260,  1261,   813,   908,  1264,  1265,   912,  1267,
    1268,  1269,  1270,  1271,   829,  1272,  1273,  1274,  1275,  1276,
     974,   928,  1284,  1285,  1308,   161,  1360,   948,  1365,  1381,
     952,  1387,  1384,  1406,  1418,  1425,   683,   686,   688,   691,
     161,  1449,  1430,   161,  1426,  1453,   888,   892,  1568,  1567,
     896,   899,  1569,  1579,   904,  1605,   704,  1606,  1625,  1424,
    1610,  1612,  1617,   718,  1620,  1437,  1632,   724,  1648,  1354,
    1633,   739,   741,   743,  1351,  1352,  1636,  1353,   759,  1637,
    1642,  1644,  1652,  1838,  1659,  1660,  1698,  1354,  1700,   788,
     790,   792,  1701,  1710,  1711,   797,   799,   802,  1721,  1732,
    1735,  1736,  1737,   815,  1744,   817,  1747,  1776,  1777,  1784,
    1783,  1780,  1785,  1788,  1796,  1786,  1801,  1808,  1847,  1839,
    1782,   842,  1429,  1580,   846,  1896,  1778,   852,  1419,  1922,
     861,   863,  1407,   868,   780,   872,   781,  1396,   877,  1397,
     881,   609,  1323,   885,   782,   669,   894,     0,  1323,  1331,
       0,  -814,     0,     0,  1331,     0,     0,   692,     0,     0,
       0,     0,     0,     0,     0,     0,   921,   923,   925,  1900,
     744,     0,     0,     0,   705,  1339,   957,     0,     0,     0,
       0,   719,     0,     0,     0,   152,   727,  1149,   735,   740,
     742,     0,   746,     0,   944,   755,     0,  1339,  -826,   642,
    1341,   711,   659,     0,     0,     0,     0,   789,     0,   667,
       0,     0,  -822,     0,  -816,   803,     0,     0,     0,     0,
    -828,  1323,  1341,     0,  -824,     0,     0,     0,  1324,   826,
       0,     0,  1325,  -820,  1324,     0,  1203,     0,  1325,  1424,
     949,     0,   847,     0,   851,  1426,   958,   858,     0,     0,
    -818,   730,     0,   738,     0,     0,     0,     0,   949,     0,
       0,  1841,   886,   890,   945,     0,     0,   897,  1323,     0,
     903,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,  1323,     0,  1331,   814,     0,     0,     0,     0,
    1331,     0,     0,     0,  1331,   830,     0,     0,     0,     0,
     959,   960,     0,  1331,     0,     0,     0,  1324,     0,     0,
       0,  1325,     0,  -814,  -814,     0,     0,     0,   946,   947,
    1323,     0,     0,     0,     0,     0,     0,   889,   893,     0,
       0,     0,     0,     0,     0,   905,     0,     0,     0,  1309,
       0,  1317,  1321,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1324,     0,  1357,     0,  1325,     0,
    -826,  -826,     0,  1361,     0,     0,     0,  1363,  1324,     0,
    1321,     0,  1325,     0,  -822,  -822,  -816,  -816,     0,   961,
    1367,     0,  -828,  -828,  1367,     0,  -824,  -824,     0,  1369,
       0,     0,     0,     0,  1374,  -820,  -820,  1377,     0,     0,
       0,     0,  1379,     0,   948,     0,  1324,  1148,     0,     0,
    1325,  1402,  -818,  -818,     0,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,     0,  1390,     0,     0,  1403,     0,
    1354,  1392,     0,     0,     0,     0,     0,  1349,  1350,  1351,
    1352,     0,  1353,   699,  1394,     0,     0,     0,     0,  1398,
       0,     0,  1354,  1400,     0,     0,   706,     0,     0,     0,
    1404,     0,     0,     0,     0,     0,     0,     0,   728,     0,
     736,     0,     0,   949,   747,     0,     0,   756,     0,     0,
       0,     0,     0,     0,     0,   783,     0,  1126,  1128,     0,
       0,     0,  1332,     0,  1333,     0,     0,   804,     0,     0,
    1408,     0,     0,  1334,  1335,  1336,     0,     0,     0,     0,
    1319,     0,   828,     0,     0,  1448,     0,  1411,  1410,     0,
       0,     0,     0,     0,  1319,     0,  1414,     0,   856,   859,
       0,  1417,     0,     0,     0,  1319,     0,     0,  1319,     0,
       0,     0,     0,     0,   887,   891,     0,     0,  1319,   898,
       0,     0,  1319,  1377,     0,     0,     0,  1319,     0,     0,
       0,     0,     0,     0,     0,  1319,     0,     0,     0,     0,
    1319,     0,     0,     0,     0,     0,     0,     0,   928,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1319,     0,     0,     0,     0,     0,  1319,
       0,  1420,     0,     0,     0,  1127,     0,     0,     0,     0,
    1422,     0,  1319,     0,     0,     0,     0,  1319,     0,  1423,
       0,  1319,     0,     0,     0,     0,     0,     0,  1319,  1411,
    1427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1431,  1432,  1433,  1434,  1435,     0,  1436,  1438,     0,     0,
    1439,  1440,     0,  1441,  1442,  1443,  1444,  1445,     0,  1446,
    1447,     0,     0,     0,  1450,  1451,     0,     0,  1319,  1454,
    1455,     0,  1458,     0,     0,     0,  1459,     0,     0,     0,
    1460,  1461,     0,     0,     0,  1319,  1462,  1463,  1464,  1465,
       0,     0,     0,     0,  1319,     0,     0,     0,     0,     0,
       0,  1466,  1467,  1468,  1469,     0,  1470,  1471,  1472,  1473,
       0,     0,  1373,     0,     0,     0,     0,     0,     0,     0,
       0,  1319,     0,  1337,     0,     0,  1338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1319,
       0,     0,  1340,     0,     0,     0,     0,     0,     0,     0,
    1341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,  1319,     0,     0,
       0,     0,     0,   161,     0,     0,     0,     0,  1343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1474,  1475,     0,     0,     0,  1476,   161,
    1477,     0,     0,     0,  1478,  1479,  1480,  1481,     0,  1456,
       0,     0,     0,     0,  1482,  1483,     0,     0,     0,  1416,
       0,     0,     0,     0,  1485,     0,  1486,  1487,  1488,   670,
    1344,     0,     0,  1490,  1493,  1494,  1495,  1496,  1497,     0,
    1498,     0,     0,  1581,     0,     0,  1499,  1500,     0,  1503,
    1504,  1505,  1506,  1507,  1508,  1509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1510,     0,
    1511,     0,     0,   670,     0,  1147,     0,     0,     0,     0,
       0,  1513,  1514,  1515,     0,  1516,     0,  1517,     0,  1518,
       0,  1519,     0,     0,  1520,  1521,   670,  1523,  1524,     0,
    1528,  1529,   949,  1530,  1531,  1532,     0,     0,     0,     0,
       0,  1533,     0,  1535,  1536,  1538,  1539,     0,     0,  1540,
       0,  1541,  1542,  1543,  1544,  1545,  1546,  1547,  1548,  1549,
    1550,     0,     0,  1551,  1552,     0,  1553,  1554,  1555,  1556,
    1557,  1558,  1559,  1560,  1561,  1562,     0,     0,     0,  1345,
       0,     0,     0,  1563,  1564,     0,     0,     0,     0,     0,
       0,  1358,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,     0,     0,  1571,   744,     0,     0,
    1354,  1317,  1566,  1319,     0,     0,     0,  1627,     0,     0,
       0,  1628,     0,  1438,  1578,     0,     0,     0,     0,     0,
       0,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,     0,     0,     0,  1607,     0,  1578,
       0,     0,     0,  1611,     0,     0,     0,  1613,     0,  1614,
    1615,  1616,     0,     0,  1619,     0,     0,  1621,     0,  1622,
    1623,   160,     0,     0,     0,     0,   663,     0,     0,     0,
    1629,     0,  1630,     0,  1631,     0,     0,     0,  1634,     0,
    1635,     0,     0,     0,   694,  1578,     0,     0,  1643,     0,
       0,  1594,  1646,  1645,  1647,     0,     0,  1650,     0,     0,
    1651,   708,   709,   710,   712,   716,   717,  1594,     0,     0,
     162,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   762,   767,   771,   775,     0,   160,     0,
       0,   712,  1438,   696,     0,     0,   794,     0,   796,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   694,     0,     0,
       0,     0,  1491,  1692,     0,     0,     0,     0,     0,     0,
       0,     0,   763,   768,   772,   776,  1501,   162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   900,     0,     0,     0,     0,   694,
       0,     0,   712,     0,     0,     0,   833,     0,     0,     0,
       0,     0,     0,     0,     0,   927,     0,     0,     0,   160,
       0,     0,     0,     0,     0,     0,     0,  1525,  1527,     0,
       0,   713,     0,     0,   160,   713,     0,   160,     0,     0,
       0,  1534,     0,  1537,     0,     0,     0,  1646,   909,  1599,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   929,     0,     0,     0,   162,     0,
       0,     0,     0,     0,     0,  1489,     0,     0,     0,     0,
    1492,     0,     0,   162,     0,     0,   162,     0,     0,     0,
       0,     0,     0,     0,  1502,     0,  1790,  1791,     0,     0,
       0,     0,     0,     0,     0,  1565,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1512,
       0,     0,     0,     0,     0,     0,     0,  1804,  1805,     0,
       0,     0,  1807,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1522,     0,  1809,  1526,     0,  1810,  1811,  1812,
       0,  1813,     0,  1814,     0,  1815,     0,  1816,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1817,
    1818,     0,  1820,     0,     0,     0,  1821,   968,     0,     0,
       0,     0,  1822,  1823,     0,     0,     0,  1824,     0,     0,
       0,     0,     0,     0,     0,     0,  1825,  1826,     0,     0,
       0,  1827,     0,  1828,     0,     0,     0,     0,     0,  1829,
    1830,     0,  1831,  1832,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   969,     0,     0,     0,
    1833,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1834,  1835,     0,     0,     0,     0,     0,     0,  1836,
       0,     0,     0,     0,     0,     0,     0,     0,  1837,     0,
       0,     0,     0,     0,  1578,     0,  1842,     0,  1843,     0,
       0,  1844,  1845,  1846,     0,     0,  1848,  1849,  1850,  1851,
    1332,  1852,  1853,     0,     0,  1854,     0,  1856,     0,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,  1872,     0,
       0,     0,  1332,     0,  1333,     0,     0,     0,     0,     0,
       0,     0,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1907,     0,     0,
    1908,     0,  1909,     0,  1910,     0,     0,  1911,     0,     0,
       0,     0,  1912,  1913,     0,  1914,     0,  1915,     0,     0,
    1916,  1917,     0,     0,     0,  1918,  1919,  1920,     0,  1921,
       0,     0,     0,     0,  1923,     0,  1924,  1925,  1926,     0,
    1927,  1928,  1929,  1930,  1931,     0,  1332,     0,  1333,     0,
       0,     0,     0,     0,     0,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1332,
       0,  1333,     0,     0,     0,     0,  1954,     0,  1955,     0,
    1334,  1335,  1336,  1956,     0,     0,  1957,     0,     0,  1958,
    1959,  1960,  1961,     0,     0,     0,     0,     0,     0,     0,
       0,  1970,     0,     0,     0,     0,  1971,  1972,  1973,     0,
    1262,     0,     0,     0,  1977,  1978,     0,     0,  1980,  1332,
    1982,  1333,  1984,     0,     0,     0,     0,     0,     0,     0,
    1334,  1335,  1336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1263,
       0,     0,     0,     0,  1318,     0,     0,     0,     0,     0,
       0,  1337,     0,     0,     0,     0,     0,     0,  1318,     0,
       0,     0,     0,  1359,     0,     0,     0,     0,     0,  1318,
       0,     0,  1318,  1337,     0,     0,  1338,  1332,     0,  1333,
       0,     0,  1318,  1339,     0,     0,  1318,     0,  1334,  1335,
    1336,  1318,     0,  1320,     0,     0,     0,     0,     0,  1318,
    1340,     0,     0,     0,  1318,  1339,     0,  1320,  1341,     0,
       0,     0,  1386,     0,     0,     0,     0,     0,  1320,     0,
    1342,  1320,  1340,     0,     0,     0,     0,  1318,     0,     0,
    1341,  1320,     0,  1318,     0,  1320,     0,     0,     0,     0,
    1320,     0,  1342,     0,     0,     0,  1318,     0,  1320,     0,
       0,  1318,     0,  1320,     0,  1318,     0,  1337,  1343,     0,
    1338,     0,  1318,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1320,     0,     0,     0,
    1337,     0,  1320,  1338,     0,     0,     0,     0,     0,  1339,
       0,     0,     0,     0,     0,  1320,     0,     0,  1618,     0,
    1320,     0,  1318,     0,  1320,     0,  1340,     0,     0,     0,
    1344,  1320,  1339,     0,  1341,     0,     0,     0,     0,  1318,
       0,     0,     0,     0,     0,     0,  1342,     0,  1318,  1340,
    1337,     0,     0,  1338,     0,     0,     0,  1341,     0,     0,
       0,     0,  1343,     0,     0,     0,     0,     0,     0,  1342,
       0,  1320,     0,     0,     0,  1318,     0,     0,  1332,     0,
    1333,     0,  1339,     0,     0,  1343,     0,     0,  1320,  1334,
    1335,  1336,     0,     0,     0,     0,     0,  1320,     0,  1340,
       0,     0,     0,     0,     0,     0,     0,  1341,     0,     0,
       0,     0,     0,     0,  1344,     0,     0,     0,     0,  1342,
       0,     0,     0,  1318,  1320,     0,     0,     0,  1337,     0,
       0,  1338,     0,     0,     0,  1343,     0,  1344,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,
       0,  1318,     0,     0,     0,     0,     0,   160,  1354,     0,
    1339,  1364,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,  1320,     0,     0,     0,  1571,  1340,     0,     0,
    1354,     0,     0,   160,     0,  1341,     0,  1344,  1452,     0,
       0,     0,     0,   694,     0,     0,     0,  1342,     0,     0,
    1320,     0,     0,     0,     0,     0,   162,     0,     0,     0,
       0,     0,     0,  1343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,  1345,     0,     0,     0,   929,     0,     0,
       0,     0,  1457,     0,     0,  1368,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,  1345,     0,     0,     0,
    1571,     0,     0,     0,  1354,  1344,     0,     0,  1370,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
       0,     0,     0,  1571,     0,     0,     0,  1354,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1345,     0,     0,  1337,
       0,     0,  1338,     0,     0,     0,     0,     0,  1368,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1354,     0,     0,
       0,  1339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   675,   676,   678,     0,     0,  1340,   679,
     682,     0,     0,     0,     0,     0,  1341,  1318,     0,   701,
       0,     0,     0,     0,     0,     0,     0,     0,  1342,   702,
       0,     0,     0,     0,  1345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1343,     0,  1378,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,  1571,   786,     0,     0,  1354,  1320,   795,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     820,   822,   823,   824,   825,     0,     0,     0,   834,     0,
       0,   836,   838,     0,     0,     0,  1344,     0,     0,   849,
       0,     0,   855,     0,   860,     0,   865,   867,   870,     0,
     874,     0,     0,   879,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   914,   915,     0,   918,   919,     0,
       0,     0,     0,     0,     0,     0,   932,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1380,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,  1571,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,   712,     0,   712,     0,     0,     0,   712,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1638,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   975,   976,   978,   979,   980,     0,     0,
     983,   984,   985,   986,     0,  1639,     0,     0,   991,   992,
     993,   994,   995,   996,   997,   998,   999,  1000,  1001,     0,
       0,     0,  1006,     0,     0,     0,  1011,  1012,  1013,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1034,
    1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,
    1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,
    1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,
    1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,     0,
       0,     0,     0,     0,  1130,  1132,  1133,  1134,     0,     0,
       0,     0,  1139,  1140,  1141,  1142,  1143,  1144,     0,     0,
    1150,  1151,  1152,  1153,  1154,  1156,     0,     0,     0,     0,
    1161,  1162,  1163,     0,     0,     0,  1795,     0,  1170,  1172,
    1173,  1174,  1175,  1176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1187,  1188,  1189,  1190,  1191,  1192,
    1193,  1194,  1195,  1196,  1197,  1198,  1200,  1202,     0,     0,
    1205,  1206,  1207,  1208,  1209,  1210,  1211,     0,     0,     0,
    1215,     0,  1217,  1219,  1221,  1223,  1224,     0,     0,     0,
       0,     0,     0,     0,     0,  1233,     0,     0,  1237,  1238,
    1239,  1240,  1241,  1243,     0,     0,     0,  1248,  1249,  1251,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1266,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1277,  1278,  1279,  1280,  1281,  1282,  1283,
       0,     0,  1286,  1287,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,
    1304,  1305,  1306,  1307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,   315,   316,     0,
       0,     0,     0,     0,     0,     0,   317,   318,   319,   320,
       0,   321,     0,     0,  1840,   322,   323,   324,   325,   326,
       0,     0,     0,   327,     0,     0,   328,     0,   329,     0,
     330,     0,   331,   332,     0,     0,   333,     0,     0,     0,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,     0,     0,     0,   348,   349,   350,
       0,     0,   351,   352,   353,   354,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   712,     0,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   468,     0,     0,     0,   469,   470,   471,     0,
       0,   472,     0,     0,     0,   473,   474,   475,   476,   477,
     478,   479,   480,     0,   481,   482,   483,   484,   485,   486,
     487,   488,     0,     0,     0,   489,     0,     0,     0,   490,
     491,   492,     0,   493,     0,     0,     0,     0,   494,     0,
       0,     0,   495,   496,   497,   498,     0,     0,     0,     0,
       0,   499,   500,   501,   502,     0,     0,   503,     0,     0,
       0,     0,   504,   505,     0,   506,   507,   508,     0,   509,
     510,   511,   512,   513,   514,     0,     0,   515,   516,   517,
     518,     0,   519,     0,     0,   520,   521,   522,   523,     0,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
       0,     0,   534,     0,     0,   535,     0,   536,     0,     0,
     537,     0,     0,     0,     0,   538,     0,     0,   539,   540,
     541,   542,   543,   544,     0,     0,     0,     0,   545,     0,
     546,     0,     0,   547,     0,     0,     0,     0,     0,     0,
     548,   549,     0,   550,   551,   552,   553,   554,     0,   555,
       0,     0,     0,     0,     0,   556,   557,   558,     0,     0,
     559,   560,   561,   562,   563,     0,   564,     0,     0,     0,
       0,   565,     0,     0,     0,     0,     0,   566,   567,   568,
     569,   570,     0,     0,     0,     0,   571,   572,   573,   574,
       0,     0,     0,   575,     0,   576,   577,     0,   578,     0,
       0,     0,     0,     0,   579,     0,   580,     0,     0,     0,
     581,     0,     0,   582,   583,     0,     0,   584,   585,   586,
     587,   588,     0,     0,   589,     0,     0,     0,   590,     0,
     591,   592,     0,     0,     0,   593,   594,   595,   596,   597,
     598,   599,   600,     0,   601,   602,   603,   604,   605,   606,
     607,   608,     0,     0,   609,     0,   610,     0,   611,     0,
     612,   613,   614,   615,   616,   617,   618,   619,   620,   621,
     622,   623,   624,   625,   626,   627,     0,     0,   628,     0,
     629,     0,     0,     0,   630,     0,     0,     0,     0,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   152,     0,
    1572,     0,     0,     0,   640,   641,     0,     0,     0,     0,
       0,  1437,   642,     0,   643,   314,   315,   316,  1573,     0,
       0,     0,     0,     0,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
    1574,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   643,  1575,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
    1574,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   643,  1608,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
    1574,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   643,  1640,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   643,  1365,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   685,  1365,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   698,  1365,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,  1371,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,  1372,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   685,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
    1437,   642,     0,   643,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,  1779,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   643,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
    1574,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   643,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   643,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   657,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   668,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   671,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   681,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   685,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   690,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   698,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   703,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   726,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   732,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   745,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   750,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   754,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   761,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   801,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   818,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   827,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   854,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   902,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,   930,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,   642,     0,  1146,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     0,     0,
       0,     0,     0,   640,   641,   314,   315,   316,     0,     0,
       0,  1312,     0,   643,     0,   317,   318,   319,   320,     0,
     321,     0,     0,     0,   322,   323,   324,   325,   326,     0,
       0,     0,   327,     0,     0,   328,     0,   329,     0,   330,
       0,   331,   332,     0,     0,   333,     0,     0,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,     0,     0,     0,   348,   349,   350,     0,
       0,   351,   352,   353,   354,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   356,
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
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,     0,     0,     0,   469,   470,   471,     0,     0,
     472,     0,     0,     0,   473,   474,   475,   476,   477,   478,
     479,   480,     0,   481,   482,   483,   484,   485,   486,   487,
     488,     0,     0,     0,   489,     0,     0,     0,   490,   491,
     492,     0,   493,     0,     0,     0,     0,   494,     0,     0,
       0,   495,   496,   497,   498,     0,     0,     0,     0,     0,
     499,   500,   501,   502,     0,     0,   503,     0,     0,     0,
       0,   504,   505,     0,   506,   507,   508,     0,   509,   510,
     511,   512,   513,   514,     0,     0,   515,   516,   517,   518,
       0,   519,     0,     0,   520,   521,   522,   523,     0,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,     0,
       0,   534,     0,     0,   535,     0,   536,     0,     0,   537,
       0,     0,     0,     0,   538,     0,     0,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,   545,     0,   546,
       0,     0,   547,     0,     0,     0,     0,     0,     0,   548,
     549,     0,   550,   551,   552,   553,   554,     0,   555,     0,
       0,     0,     0,     0,   556,   557,   558,     0,     0,   559,
     560,   561,   562,   563,     0,   564,     0,     0,     0,     0,
     565,     0,     0,     0,     0,     0,   566,   567,   568,   569,
     570,     0,     0,     0,     0,   571,   572,   573,   574,     0,
       0,     0,   575,     0,   576,   577,     0,   578,     0,     0,
       0,     0,     0,   579,     0,   580,     0,     0,     0,   581,
       0,     0,   582,   583,     0,     0,   584,   585,   586,   587,
     588,     0,     0,   589,     0,     0,     0,   590,     0,   591,
     592,     0,     0,     0,   593,   594,   595,   596,   597,   598,
     599,   600,     0,   601,   602,   603,   604,   605,   606,   607,
     608,     0,     0,   609,     0,   610,     0,   611,     0,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,     0,     0,   628,     0,   629,
       0,     0,     0,   630,     0,     0,     0,     0,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   152,     1,     0,
       0,     0,     2,     3,     0,     0,     0,     0,     4,     0,
       0,   642,     0,   643,     0,     0,     0,     0,     5,     6,
       7,     0,     8,     9,     0,    10,     0,    11,     0,    12,
       0,     0,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,    19,    20,     0,     0,     0,    21,    22,
       0,     0,     0,     0,    23,     0,     0,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1332,     0,  1333,     0,
       0,     0,     0,     0,     0,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1332,     0,  1333,     0,     0,     0,     0,     0,     0,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,     0,     0,     0,    38,     0,     0,
      39,    40,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    44,    45,     0,    46,    47,    48,     0,     0,     0,
       0,     0,    49,     0,    50,    51,     0,    52,    53,    54,
       0,     0,     0,     0,    55,    56,    57,    58,    59,     0,
       0,     0,     0,    60,    61,     0,    62,    63,    64,    65,
       0,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,     0,
       0,    69,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    71,    72,     0,    73,     0,    74,    75,     0,    76,
      77,    78,    79,     0,    80,     0,     0,     0,     0,     0,
       0,     0,    81,    82,    83,    84,     0,    85,     0,    86,
      87,     0,    88,    89,    90,    91,    92,    93,     0,     0,
      94,     0,     0,     0,     0,     0,    95,  1337,    96,     0,
      97,    98,    99,     0,     0,     0,   100,   101,     0,     0,
       0,     0,     0,   102,     0,   103,   104,     0,     0,     0,
     105,   106,   107,   108,   109,     0,     0,     0,     0,  1339,
     110,   111,   112,   113,     0,     0,     0,     0,   114,   115,
     116,  1337,   117,     0,  1338,   118,  1340,   119,   120,   121,
     122,   123,     0,   124,  1341,   125,   126,   127,     0,   128,
     129,     0,     0,   130,   131,     0,  1342,     0,     0,     0,
     132,     0,     0,  1339,   133,   134,     0,   135,     0,     0,
       0,   136,  1343,     0,     0,     0,     0,     0,     0,     0,
    1340,   137,     0,     0,     0,     0,     0,     0,  1341,     0,
     138,   139,     0,   140,     0,   141,     0,   142,     0,     0,
    1342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   143,   144,  1343,   145,     0,   146,
     147,   148,     0,   149,   150,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   152,   153,     1,     0,
       0,     0,     2,     3,     0,     0,     0,     0,     4,     0,
       0,     0,   154,     0,     0,     0,   155,     0,     5,     6,
       7,     0,     8,     9,     0,    10,     0,    11,  1344,    12,
       0,     0,    13,    14,     0,    15,    16,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,    19,    20,     0,     0,     0,    21,    22,
       0,     0,     0,     0,    23,     0,     0,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1391,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,     0,     0,  1571,     0,     0,     0,  1354,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,     0,     0,     0,    38,     0,     0,
      39,    40,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    44,    45,     0,    46,    47,    48,     0,     0,     0,
       0,     0,    49,     0,    50,    51,     0,    52,    53,    54,
       0,     0,     0,     0,    55,    56,    57,    58,    59,     0,
       0,     0,     0,    60,    61,     0,    62,    63,    64,    65,
       0,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,     0,
       0,    69,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    71,    72,     0,    73,     0,    74,    75,     0,    76,
      77,    78,    79,     0,    80,     0,     0,     0,     0,     0,
       0,     0,    81,    82,    83,    84,     0,    85,     0,    86,
      87,     0,    88,    89,    90,    91,    92,    93,     0,     0,
      94,     0,     0,     0,     0,     0,    95,     0,    96,     0,
      97,    98,   937,     0,     0,     0,   100,   101,     0,     0,
       0,     0,     0,   102,     0,   103,   104,     0,     0,     0,
     105,   106,   107,   108,   109,     0,     0,     0,     0,     0,
     110,   111,   112,   113,     0,     0,     0,     0,   114,   115,
     116,     0,   117,     0,     0,   118,     0,   119,   120,   121,
     122,   123,     0,   124,     0,   125,   126,   127,     0,   128,
     129,     0,     0,   130,   131,     0,     0,     0,     0,     0,
     132,     0,     0,     0,   133,   134,     0,   135,     0,     0,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   137,     0,     0,     0,     0,     0,     0,     0,     0,
     138,   139,     0,   140,     0,   141,     0,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     0,     0,     0,
       2,     3,     0,     0,   143,   144,     4,   145,     0,   146,
     147,   148,     0,   149,   150,   151,     5,     6,     7,     0,
       8,     9,     0,    10,     0,    11,   152,    12,     0,     0,
      13,    14,     0,    15,    16,    17,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,   155,     0,     0,     0,
      18,    19,    20,     0,     0,     0,    21,    22,     0,     0,
       0,     0,    23,     0,     0,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1332,     0,
    1333,     0,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
      36,    37,     0,     0,     0,    38,     0,     0,    39,    40,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    44,
      45,     0,    46,    47,    48,     0,     0,     0,     0,     0,
      49,     0,    50,    51,     0,    52,    53,    54,     0,     0,
       0,     0,    55,    56,    57,    58,    59,     0,     0,     0,
       0,    60,    61,     0,    62,    63,    64,    65,     0,     0,
       0,     0,     0,     0,    66,  1332,     0,  1333,     0,     0,
       0,    67,    68,     0,     0,     0,  1334,  1335,  1336,    69,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    71,
      72,     0,    73,     0,    74,    75,     0,    76,    77,    78,
      79,     0,    80,     0,     0,     0,     0,     0,     0,     0,
      81,    82,    83,    84,     0,    85,     0,    86,    87,     0,
      88,    89,    90,    91,    92,    93,     0,     0,    94,  1332,
       0,  1333,     0,     0,    95,     0,    96,     0,    97,    98,
    1334,  1335,  1336,     0,   100,   101,     0,     0,     0,     0,
       0,   102,     0,   103,   104,     0,     0,     0,   105,   106,
     107,   108,   109,     0,     0,     0,     0,     0,   110,   111,
     112,   113,     0,     0,     0,     0,   114,   115,   116,  1337,
     117,     0,  1338,   118,     0,   119,   120,   121,   122,   123,
       0,   124,     0,   125,   126,   127,     0,   128,   129,     0,
       0,   130,   131,     0,     0,     0,     0,     0,   132,     0,
       0,  1339,   133,   134,     0,   135,     0,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,  1340,   137,
       0,     0,     0,     0,     0,     0,  1341,     0,   138,   139,
       0,   140,     0,   141,     0,   142,     0,     0,  1342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   143,   144,  1343,   145,     0,   146,   147,   148,
       0,   149,   150,   151,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   152,     0,  1337,     0,     0,  1338,
    1332,     0,  1333,     0,     0,     0,     0,     0,     0,     0,
     154,  1334,  1335,  1336,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1344,     0,  1339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
    1337,     0,  1332,  1338,  1333,     0,     0,     0,     0,     0,
       0,  1343,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1332,     0,  1333,     0,
       0,     0,  1339,     0,     0,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1340,
       0,     0,     0,     0,     0,     0,     0,  1341,     0,     0,
    1332,     0,  1333,  1344,     0,     0,     0,     0,     0,  1342,
       0,  1334,  1335,  1336,     0,  1345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1343,     0,  1393,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,  1332,     0,
    1333,     0,  1571,     0,     0,     0,  1354,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1337,     0,     0,  1338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1332,     0,  1333,     0,     0,     0,
       0,     0,  1345,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,  1339,  1395,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,     0,  1571,
    1340,     0,     0,  1354,     0,     0,     0,     0,  1341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1342,     0,     0,  1337,     0,     0,  1338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1345,  1337,     0,     0,
    1338,     0,     0,     0,     0,  1339,     0,     0,  1399,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
       0,     0,  1340,  1571,     0,     0,     0,  1354,     0,  1339,
    1341,  1337,     0,     0,  1338,     0,     0,     0,  1344,     0,
       0,     0,  1342,     0,     0,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,     0,     0,  1343,     0,
       0,     0,     0,  1339,     0,     0,  1342,     0,     0,  1337,
       0,     0,  1338,     0,     0,     0,     0,     0,     0,     0,
    1340,     0,  1343,     0,     0,     0,     0,     0,  1341,     0,
       0,     0,  1332,     0,  1333,     0,     0,     0,     0,     0,
    1342,  1339,     0,  1334,  1335,  1336,     0,     0,     0,     0,
    1344,     0,     0,  1649,     0,     0,  1343,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,  1341,     0,     0,     0,
       0,     0,     0,     0,  1344,     0,     0,     0,  1342,     0,
       0,     0,     0,     0,     0,  1337,     0,  1332,  1338,  1333,
       0,     0,     0,     0,  1343,     0,     0,  1345,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,  1344,  1401,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1339,  1353,     0,
       0,     0,     0,     0,  1571,     0,     0,     0,  1354,     0,
       0,     0,     0,     0,  1340,     0,     0,     0,     0,     0,
       0,     0,  1341,     0,     0,  1332,  1344,  1333,     0,     0,
       0,     0,     0,     0,  1342,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1345,
    1343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1405,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,  1345,     0,     0,  1571,     0,     0,     0,
    1354,     0,     0,     0,     0,  1409,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
    1571,     0,  1344,     0,  1354,     0,     0,  1345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1413,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,     0,     0,  1571,     0,     0,     0,  1354,  1332,
       0,  1333,     0,  1337,     0,  1345,  1338,     0,     0,     0,
    1334,  1335,  1336,     0,     0,     0,     0,  1415,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,  1571,     0,     0,  1339,  1354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1340,     0,  1332,     0,  1333,     0,  1337,     0,
    1341,  1338,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,  1345,     0,     0,     0,     0,     0,     0,  1343,     0,
    1339,     0,     0,  1368,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,  1340,     0,     0,
       0,     0,  1354,     0,     0,  1341,  1337,     0,  1332,  1338,
    1333,     0,     0,     0,     0,     0,     0,  1342,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
    1344,     0,     0,  1343,  1332,     0,  1333,     0,  1339,     0,
       0,     0,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,     0,  1340,  1332,     0,  1333,     0,
       0,     0,     0,  1341,     0,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1344,     0,     0,     0,     0,
       0,  1343,     0,     0,     0,     0,     0,     0,  1332,     0,
    1333,     0,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
    1337,     0,     0,  1338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1344,     0,     0,     0,     0,     0,  1345,
       0,     0,  1339,     0,     0,     0,     0,     0,     0,     0,
       0,  1421,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1340,
    1353,     0,     0,     0,     0,  1337,  1571,  1341,  1338,  1332,
    1354,  1333,     0,     0,     0,     0,     0,     0,     0,  1342,
    1334,  1335,  1336,     0,     0,     0,     0,     0,     0,     0,
       0,  1332,     0,  1333,  1345,  1343,     0,  1339,     0,     0,
       0,     0,  1334,  1335,  1336,     0,  1653,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,  1340,  1353,     0,  1654,     0,     0,
       0,     0,  1341,     0,     0,  1354,     0,     0,     0,  1337,
       0,     0,     0,     0,  1342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1344,     0,     0,
    1343,     0,  1345,     0,     0,  1337,     0,     0,  1338,     0,
       0,  1339,     0,     0,  1665,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,  1337,  1340,  1666,
    1338,     0,     0,  1354,     0,     0,  1341,  1339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1342,     0,
       0,     0,  1344,     0,  1340,     0,     0,     0,     0,  1339,
       0,     0,  1341,     0,     0,     0,     0,     0,     0,  1337,
       0,     0,  1338,     0,  1342,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,  1332,     0,  1333,     0,
    1343,     0,     0,     0,     0,     0,  1342,  1334,  1335,  1336,
       0,  1339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1343,     0,     0,     0,  1345,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,  1341,     0,  1670,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1342,     0,
       0,     0,  1344,  1671,     0,     0,     0,  1354,     0,     0,
    1337,     0,     0,  1338,  1343,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1344,     0,     0,     0,     0,     0,
       0,  1345,  1337,     0,     0,  1338,     0,     0,     0,     0,
       0,     0,  1339,  1672,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,     0,  1673,  1340,
       0,  1332,  1354,  1333,  1339,     0,  1344,  1341,     0,     0,
       0,     0,  1334,  1335,  1336,     0,     0,     0,     0,  1342,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,  1341,
       0,  1332,     0,  1333,     0,  1343,     0,     0,     0,     0,
       0,  1342,  1334,  1335,  1336,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,  1343,     0,     0,
       0,  1345,     0,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,  1674,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1345,     0,     0,     0,  1344,  1675,     0,
       0,     0,  1354,     0,     0,  1676,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,  1344,
    1677,     0,     0,     0,  1354,     0,     0,  1337,     0,     0,
    1338,  1332,     0,  1333,     0,  1345,     0,     0,     0,     0,
       0,     0,  1334,  1335,  1336,     0,     0,  1678,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,  1339,
       0,     0,  1679,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,  1332,     0,  1333,     0,
       0,     0,     0,     0,     0,     0,  1342,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,     0,  1332,     0,
    1333,     0,  1343,     0,     0,     0,  1345,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,  1484,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,     0,
       0,     0,  1337,  1571,     0,  1338,     0,  1354,     0,     0,
    1693,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,     0,  1344,  1694,     0,     0,     0,  1354,
       0,     0,  1337,     0,  1339,  1338,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,  1341,
       0,  1332,     0,  1333,  1339,     0,     0,     0,     0,     0,
       0,  1342,  1334,  1335,  1336,     0,     0,     0,     0,     0,
       0,  1340,     0,     0,     0,     0,     0,  1343,     0,  1341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1343,     0,     0,
       0,     0,  1337,     0,  1332,  1338,  1333,     0,     0,     0,
       0,     0,     0,     0,     0,  1334,  1335,  1336,     0,  1344,
       0,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1339,  1370,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,  1344,
    1702,  1340,     0,     0,  1354,     0,     0,  1337,     0,  1341,
    1338,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1342,     0,     0,     0,     0,     0,     0,     0,  1337,
       0,     0,  1338,  1332,     0,  1333,     0,  1343,     0,  1339,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,     0,     0,  1332,  1340,  1333,     0,     0,
       0,  1339,     0,     0,  1341,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,  1342,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,  1341,     0,  1345,  1344,
       0,     0,  1343,     0,     0,     0,     0,     0,  1342,     0,
    1724,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,     0,  1343,  1725,     0,     0,  1345,  1354,
       0,     0,  1337,     0,     0,  1338,     0,     0,     0,     0,
    1368,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,     0,  1344,  1738,     0,     0,  1332,  1354,
    1333,     0,     0,     0,  1339,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,  1344,     0,     0,     0,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,  1341,
       0,  1332,     0,  1333,     0,  1337,     0,     0,  1338,     0,
       0,  1342,  1334,  1335,  1336,     0,     0,     0,     0,     0,
       0,     0,     0,  1332,     0,  1333,     0,  1343,  1345,     0,
       0,     0,     0,     0,  1334,  1335,  1336,  1339,     0,     0,
    1368,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,     0,  1340,  1745,     0,     0,     0,  1354,
       0,     0,  1341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1342,     0,     0,     0,     0,  1344,
       0,     0,     0,  1345,  1337,     0,     0,  1338,     0,     0,
    1343,     0,     0,     0,     0,  1368,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,  1345,  1337,     0,     0,  1338,
    1748,     0,     0,     0,  1354,     0,  1339,  1763,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,  1764,  1340,     0,     0,  1354,     0,  1339,     0,
       0,  1341,  1344,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1342,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,  1332,     0,  1333,     0,  1343,
       0,     0,     0,     0,     0,  1342,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1343,     0,     0,     0,  1332,     0,  1333,  1345,  1337,
       0,     0,  1338,     0,     0,     0,  1334,  1335,  1336,     0,
    1858,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,  1344,     0,     0,     0,  1859,     0,     0,     0,  1354,
       0,  1339,  1337,     0,     0,  1338,     0,     0,     0,     0,
       0,     0,     0,  1344,     0,     0,     0,     0,  1340,     0,
       0,     0,     0,     0,  1337,     0,  1341,  1338,  1332,     0,
    1333,  1345,     0,     0,  1339,     0,     0,     0,  1342,  1334,
    1335,  1336,     0,  1861,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1340,  1353,     0,  1343,     0,  1339,     0,  1862,  1341,
       0,     0,  1354,     0,     0,     0,     0,     0,     0,     0,
       0,  1342,     0,  1340,     0,     0,     0,     0,  1332,     0,
    1333,  1341,     0,     0,     0,     0,     0,  1343,     0,  1334,
    1335,  1336,     0,  1342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1344,     0,     0,  1343,
    1345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1863,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,  1345,     0,     0,     0,     0,  1864,     0,  1344,
       0,  1354,     0,     0,  1865,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,     0,  1866,
       0,  1344,     0,  1354,     0,     0,  1337,     0,     0,  1338,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1332,     0,  1333,     0,  1337,     0,  1339,  1338,
       0,     0,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,  1345,     0,     0,  1339,     0,
       0,     0,     0,     0,     0,  1342,     0,  1868,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,  1340,  1353,     0,     0,     0,
       0,  1343,  1869,  1341,     0,  1332,  1354,  1333,  1345,  1337,
       0,     0,  1338,     0,     0,  1342,  1334,  1335,  1336,     0,
    1874,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
    1345,  1343,     0,     0,     0,  1875,     0,     0,     0,  1354,
       0,  1339,  1876,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1344,     0,     0,     0,  1877,  1340,  1337,
       0,  1354,  1338,  1332,     0,  1333,  1341,     0,     0,     0,
       0,     0,     0,     0,  1334,  1335,  1336,     0,  1342,     0,
       0,     0,     0,  1344,     0,     0,  1332,     0,  1333,     0,
       0,  1339,     0,     0,  1343,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,  1341,     0,     0,  1332,
       0,  1333,     0,     0,     0,     0,     0,     0,  1342,     0,
    1334,  1335,  1336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1343,     0,  1344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1332,
       0,  1333,  1345,  1337,     0,     0,  1338,     0,     0,     0,
    1334,  1335,  1336,     0,  1878,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,  1344,     0,     0,  1879,
       0,     0,  1345,  1354,     0,  1339,     0,     0,     0,     0,
       0,     0,     0,     0,  1882,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1340,  1353,     0,     0,     0,     0,     0,  1883,
    1341,     0,  1332,  1354,  1333,     0,  1337,     0,     0,  1338,
       0,     0,  1342,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1343,     0,
       0,     0,     0,     0,     0,  1345,     0,     0,  1339,     0,
       0,     0,     0,     0,     0,     0,     0,  1890,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,  1340,  1353,     0,     0,     0,
       0,     0,  1891,  1341,  1337,     0,  1354,  1338,     0,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
    1344,     0,     0,     0,     0,  1345,     0,  1337,     0,     0,
    1338,  1343,     0,     0,     0,     0,  1339,  1939,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,  1940,  1340,     0,     0,  1354,     0,     0,  1339,
    1337,  1341,     0,  1338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1342,     0,     0,  1340,     0,     0,     0,
       0,     0,     0,  1344,  1341,     0,     0,     0,  1332,  1343,
    1333,     0,  1339,     0,     0,     0,  1342,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,  1340,
    1337,     0,  1343,  1338,  1332,     0,  1333,  1341,     0,     0,
       0,     0,     0,     0,     0,  1334,  1335,  1336,     0,  1342,
       0,     0,     0,     0,     0,     0,  1332,     0,  1333,  1345,
       0,  1344,  1339,     0,     0,  1343,     0,  1334,  1335,  1336,
       0,  1944,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1340,
    1353,     0,     0,     0,  1344,     0,  1945,  1341,     0,  1332,
    1354,  1333,     0,  1337,     0,     0,  1338,     0,     0,  1342,
    1334,  1335,  1336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1343,     0,  1344,     0,     0,
       0,     0,  1345,     0,     0,  1339,     0,     0,     0,     0,
       0,     0,     0,     0,  1947,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1340,  1353,     0,     0,     0,     0,     0,  1948,
    1341,     0,  1332,  1354,  1333,     0,     0,     0,     0,     0,
       0,     0,  1342,  1334,  1335,  1336,     0,  1344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1343,     0,
    1345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1962,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1345,     0,     0,     0,  1963,     0,     0,
       0,  1354,     0,     0,     0,  1358,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
    1344,     0,     0,     0,  1354,     0,  1345,     0,     0,  1337,
       0,     0,  1338,     0,     0,     0,     0,     0,  1364,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
       0,     0,     0,     0,     0,  1337,     0,  1354,  1338,     0,
       0,  1339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1345,  1337,  1340,     0,
    1338,     0,     0,     0,     0,     0,  1341,  1339,  1368,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1342,     0,
       0,     0,     0,     0,  1340,     0,  1332,  1354,  1333,  1339,
    1337,     0,  1341,  1338,  1343,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,  1342,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,  1332,     0,  1333,  1345,
    1343,     0,  1339,     0,     0,     0,  1342,  1334,  1335,  1336,
       0,  1370,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1340,
    1353,     0,  1343,     0,     0,     0,  1344,  1341,     0,  1332,
    1354,  1333,     0,  1337,     0,     0,  1338,     0,     0,  1342,
    1334,  1335,  1336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1344,     0,     0,  1343,  1332,     0,  1333,     0,
       0,     0,     0,     0,     0,  1339,     0,  1334,  1335,  1336,
       0,     0,     0,     0,  1344,     0,     0,     0,     0,     0,
       0,     0,  1340,     0,     0,     0,     0,     0,     0,     0,
    1341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,  1344,     0,     0,
       0,     0,  1332,     0,  1333,     0,     0,     0,  1343,     0,
       0,     0,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1389,     0,  1332,
       0,  1333,     0,     0,     0,  1345,     0,     0,     0,     0,
    1334,  1335,  1336,     0,     0,     0,     0,  1378,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
    1344,  1345,     0,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,  1380,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1345,     0,     0,     0,  1337,     0,     0,
    1338,     0,  1354,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
       0,     0,     0,  1332,  1354,  1333,  1345,  1337,     0,  1339,
    1338,     0,     0,     0,  1334,  1335,  1336,     0,  1391,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,  1340,  1353,     0,     0,
       0,     0,     0,     0,  1341,     0,     0,  1354,     0,  1339,
    1337,     0,     0,  1338,     0,     0,  1342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1340,     0,     0,     0,
       0,     0,  1343,     0,  1341,     0,     0,  1337,     0,  1345,
    1338,     0,  1339,     0,     0,     0,  1342,     0,     0,     0,
       0,  1393,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1340,
    1353,     0,  1343,     0,     0,     0,     0,  1341,     0,  1339,
    1354,     0,     0,     0,     0,     0,     0,     0,     0,  1342,
       0,     0,     0,     0,  1344,     0,  1340,     0,     0,     0,
       0,     0,     0,  1337,  1341,  1343,  1338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1342,     0,     0,     0,
       0,     0,     0,     0,  1344,     0,  1332,     0,  1333,     0,
    1337,     0,  1343,  1338,     0,  1339,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1340,  1332,     0,  1333,     0,  1344,     0,     0,
    1341,     0,  1339,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,  1340,
       0,     0,     0,     0,  1344,     0,     0,  1341,  1343,     0,
       0,     0,     0,  1332,     0,  1333,     0,     0,     0,  1342,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,  1345,  1337,  1343,     0,  1338,     0,     0,
       0,     0,     0,     0,     0,  1395,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,  1332,     0,  1333,
    1344,     0,     0,  1345,  1354,     0,  1339,     0,  1334,  1335,
    1336,     0,     0,     0,     0,  1399,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,  1340,  1353,     0,     0,  1412,     0,     0,
       0,  1341,     0,     0,  1354,     0,  1345,     0,     0,     0,
       0,     0,     0,  1342,     0,     0,     0,     0,  1401,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,  1343,
    1332,     0,  1333,  1345,     0,     0,     0,  1354,     0,     0,
       0,  1334,  1335,  1336,     0,  1405,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1354,  1332,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,     0,  1334,  1335,  1336,     0,
       0,  1344,     0,     0,     0,     0,     0,  1337,     0,  1345,
    1338,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1409,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,  1337,     0,  1345,  1338,     0,  1339,
    1354,     0,     0,     0,     0,     0,     0,     0,  1413,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,  1340,  1353,     0,     0,
       0,     0,     0,     0,  1341,     0,  1339,  1354,     0,     0,
       0,     0,     0,     0,  1337,     0,  1342,  1338,     0,     0,
       0,     0,     0,  1340,     0,     0,     0,     0,     0,     0,
       0,  1341,  1343,     0,     0,     0,     0,  1332,     0,  1333,
       0,     0,     0,  1342,     0,     0,  1339,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,  1337,  1343,
    1345,  1338,     0,  1340,     0,     0,     0,     0,     0,     0,
       0,  1341,  1415,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1342,  1344,  1332,     0,  1333,     0,     0,
    1339,  1354,     0,     0,     0,     0,  1334,  1335,  1336,  1343,
       0,     0,     0,     0,     0,     0,     0,  1340,     0,     0,
       0,  1428,     0,     0,     0,  1341,     0,     0,     0,     0,
       0,  1337,     0,     0,  1338,     0,     0,  1342,     0,     0,
       0,     0,     0,     0,     0,     0,  1332,     0,  1333,     0,
       0,     0,     0,  1343,     0,     0,     0,  1334,  1335,  1336,
       0,  1344,     0,  1339,     0,     0,  1337,     0,     0,  1338,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,  1341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1339,     0,
    1342,     0,     0,     0,     0,  1344,     0,     0,     0,     0,
       0,     0,     0,  1345,     0,  1340,  1343,     0,     0,     0,
       0,     0,     0,  1341,     0,  1421,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,  1342,     0,     0,     0,     0,
    1345,     0,     0,     0,  1354,     0,     0,     0,     0,     0,
       0,  1343,  1413,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,     0,     0,     0,     0,     0,  1344,     0,
       0,  1354,     0,     0,  1332,     0,  1333,     0,  1337,     0,
    1345,  1338,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,  1484,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1344,  1332,     0,  1333,     0,     0,     0,
    1339,  1354,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,  1345,     0,  1337,  1340,     0,  1338,
       0,     0,     0,     0,     0,  1341,  1413,  1346,  1347,  1348,
    1570,  1350,  1351,  1352,     0,  1353,     0,  1342,     0,     0,
       0,     0,     0,     0,     0,  1354,     0,     0,  1339,     0,
       0,     0,     0,  1343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1340,     0,  1337,     0,     0,
    1338,     0,     0,  1341,     0,     0,     0,  1345,     0,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1339,
       0,  1343,     0,     0,  1571,  1344,     0,     0,  1354,     0,
       0,     0,  1345,     0,     0,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,  1342,     0,  1332,  1655,
    1333,     0,     0,  1354,     0,     0,     0,     0,     0,  1334,
    1335,  1336,  1343,  1344,     0,     0,     0,     0,     0,     0,
    1332,     0,  1333,     0,     0,     0,     0,     0,     0,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1337,     0,     0,  1338,     0,
       0,     0,     0,     0,  1344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1332,     0,  1333,  1345,  1337,     0,  1339,  1338,     0,
       0,     0,  1334,  1335,  1336,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,  1340,  1353,     0,     0,     0,     0,
       0,  1656,  1341,     0,  1332,  1354,  1333,  1339,     0,     0,
       0,     0,     0,     0,  1342,  1334,  1335,  1336,     0,     0,
       0,     0,  1345,     0,  1340,     0,     0,     0,     0,     0,
    1343,     0,  1341,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1342,     0,     0,     0,     0,  1657,
       0,     0,  1332,  1354,  1333,     0,     0,     0,     0,     0,
    1343,     0,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,  1344,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
    1658,     0,     0,     0,  1354,     0,     0,     0,     0,     0,
       0,     0,  1344,     0,     0,     0,     0,     0,     0,  1337,
       0,     0,  1338,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1337,     0,     0,  1338,  1332,     0,  1333,     0,     0,
       0,  1339,     0,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,     0,  1332,  1340,  1333,
       0,     0,     0,  1339,     0,     0,  1341,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,  1342,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,  1341,     0,
       0,  1345,  1337,     0,  1343,  1338,     0,     0,     0,     0,
    1342,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,  1343,     0,  1661,     0,
       0,  1345,  1354,     0,  1339,  1337,     0,     0,  1338,     0,
       0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1340,  1353,     0,     0,     0,  1344,     0,  1662,  1341,
       0,  1332,  1354,  1333,     0,     0,     0,  1339,     0,     0,
       0,  1342,  1334,  1335,  1336,     0,     0,     0,  1344,     0,
       0,     0,     0,  1337,  1340,     0,  1338,  1343,     0,     0,
       0,     0,  1341,     0,     0,     0,  1332,     0,  1333,     0,
       0,     0,     0,     0,  1342,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,  1339,     0,     0,     0,     0,
    1343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1340,     0,     0,     0,     0,     0,     0,  1344,
    1341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1343,     0,
       0,     0,  1344,     0,     0,  1345,  1337,     0,     0,  1338,
       0,     0,     0,     0,     0,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,  1337,     0,
       0,  1338,  1663,     0,     0,     0,  1354,     0,  1339,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
    1344,     0,     0,     0,  1664,  1340,     0,  1332,  1354,  1333,
    1339,     0,     0,  1341,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,  1342,     0,  1340,     0,     0,
       0,     0,     0,     0,     0,  1341,     0,  1332,  1345,  1333,
       0,  1343,     0,     0,     0,     0,     0,  1342,  1334,  1335,
    1336,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,  1343,     0,  1667,     0,     0,     0,  1354,
       0,  1345,  1337,     0,     0,  1338,     0,     0,     0,     0,
       0,     0,     0,  1668,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1344,     0,     0,     0,     0,     0,     0,
       0,     0,  1354,     0,  1339,     0,     0,  1337,     0,     0,
    1338,     0,     0,     0,     0,  1344,     0,     0,     0,  1345,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,  1341,
       0,  1669,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1339,
    1353,  1342,     0,     0,     0,     0,     0,     0,     0,     0,
    1354,     0,     0,     0,     0,     0,  1340,  1343,  1332,     0,
    1333,     0,     0,     0,  1341,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,  1342,     0,     0,     0,
       0,     0,  1332,     0,  1333,     0,     0,     0,     0,     0,
       0,     0,  1343,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1344,
       0,     0,  1345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1345,     0,     0,     0,  1337,  1680,
       0,  1338,     0,  1354,  1344,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,  1681,     0,     0,     0,  1354,     0,     0,  1337,     0,
    1339,  1338,  1332,     0,  1333,     0,     0,     0,     0,     0,
       0,     0,     0,  1334,  1335,  1336,     0,  1340,     0,     0,
       0,     0,     0,     0,     0,  1341,     0,     0,     0,     0,
    1339,     0,  1332,     0,  1333,     0,     0,  1342,     0,     0,
       0,     0,     0,  1334,  1335,  1336,     0,  1340,     0,     0,
       0,     0,     0,  1343,     0,  1341,     0,     0,  1345,     0,
       0,     0,     0,     0,     0,     0,     0,  1342,     0,     0,
       0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,  1343,     0,  1682,     0,     0,     0,  1354,
       0,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1344,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,  1337,
    1683,     0,  1338,     0,  1354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1344,     0,     0,     0,     0,
       0,     0,     0,  1337,     0,  1332,  1338,  1333,     0,     0,
       0,  1339,     0,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1340,     0,
       0,     0,     0,     0,     0,  1339,  1341,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1342,     0,
       0,     0,  1340,     0,     0,     0,     0,     0,     0,     0,
    1341,     0,     0,     0,  1343,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1345,     0,     0,     0,  1343,     0,
       0,     0,     0,  1337,     0,     0,  1338,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,  1684,     0,     0,  1345,  1354,  1344,     0,     0,     0,
       0,     0,     0,  1337,     0,  1339,  1338,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
    1344,  1685,  1340,     0,     0,  1354,     0,     0,     0,     0,
    1341,     0,  1332,     0,  1333,  1339,     0,     0,     0,     0,
       0,     0,  1342,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,  1340,     0,  1332,     0,  1333,     0,  1343,     0,
    1341,     0,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
    1332,     0,  1333,     0,     0,     0,     0,     0,  1343,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1344,     0,     0,     0,     0,  1345,  1337,     0,     0,  1338,
       0,     0,     0,     0,     0,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,  1345,
    1344,     0,  1686,     0,     0,     0,  1354,     0,  1339,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,     0,  1340,  1687,     0,     0,     0,
    1354,     0,     0,  1341,     0,  1332,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,  1342,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,     0,  1332,     0,  1333,
       0,  1343,     0,     0,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1332,     0,  1333,     0,     0,     0,  1345,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,  1344,     0,     0,  1688,     0,     0,  1345,
    1354,     0,     0,  1337,     0,     0,  1338,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,     0,  1337,  1689,     0,  1338,     0,
    1354,     0,     0,     0,     0,  1339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1337,  1340,     0,  1338,     0,     0,  1339,     0,     0,
    1341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,  1340,     0,     0,     0,     0,  1332,
       0,  1333,  1341,  1339,     0,     0,     0,     0,  1343,     0,
    1334,  1335,  1336,     0,  1342,     0,     0,     0,     0,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,  1341,     0,
    1343,     0,  1345,     0,     0,     0,     0,     0,     0,     0,
    1342,     0,     0,     0,  1690,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,  1343,     0,     0,     0,
    1344,     0,     0,  1354,     0,     0,  1337,     0,     0,  1338,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1344,     0,     0,     0,     0,     0,  1337,     0,
       0,  1338,  1332,     0,  1333,     0,     0,     0,  1339,     0,
       0,     0,     0,  1334,  1335,  1336,     0,     0,  1344,     0,
       0,     0,     0,     0,  1337,  1340,  1332,  1338,  1333,     0,
    1339,     0,     0,  1341,     0,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,  1342,     0,  1340,     0,     0,
       0,     0,  1332,     0,  1333,  1341,  1339,     0,     0,     0,
       0,  1343,     0,  1334,  1335,  1336,     0,  1342,     0,     0,
       0,     0,     0,  1340,  1332,     0,  1333,     0,     0,     0,
       0,  1341,     0,  1343,     0,  1334,  1335,  1336,     0,  1345,
       0,     0,     0,  1342,     0,     0,     0,     0,     0,     0,
       0,  1691,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1343,
    1353,  1345,     0,  1344,     0,     0,     0,     0,     0,     0,
    1354,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,  1344,     0,  1345,  1695,     0,
    1337,     0,  1354,  1338,     0,     0,     0,     0,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,  1344,     0,     0,  1696,     0,     0,  1332,  1354,  1333,
       0,     0,  1339,     0,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,     0,  1340,
       0,     0,     0,     0,     0,     0,     0,  1341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1343,     0,     0,     0,     0,
       0,     0,  1345,  1337,     0,     0,  1338,     0,     0,     0,
       0,     0,     0,     0,  1697,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1345,     0,     0,  1337,     0,     0,
    1338,     0,     0,  1354,     0,  1339,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,  1344,     0,     0,
    1345,  1699,  1340,  1337,     0,  1354,  1338,     0,     0,  1339,
    1341,     0,  1703,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,  1342,     0,     0,  1337,  1340,     0,  1338,     0,
       0,  1354,     0,     0,  1341,  1339,     0,     0,  1343,     0,
       0,     0,     0,     0,     0,     0,  1342,     0,     0,     0,
       0,     0,  1340,  1332,     0,  1333,     0,  1339,     0,     0,
    1341,     0,  1343,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,  1342,     0,  1340,     0,     0,     0,     0,     0,
       0,     0,  1341,     0,     0,     0,     0,     0,  1343,     0,
    1344,     0,     0,     0,  1342,     0,  1332,     0,  1333,     0,
       0,     0,     0,     0,     0,     0,     0,  1334,  1335,  1336,
    1343,     0,     0,     0,  1344,     0,  1345,     0,  1337,     0,
       0,  1338,     0,     0,     0,     0,     0,     0,  1704,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
    1344,     0,     0,     0,  1332,     0,  1333,  1354,     0,     0,
    1339,     0,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,  1344,     0,     0,     0,  1332,  1340,  1333,     0,
       0,     0,     0,     0,     0,  1341,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,  1342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1332,     0,  1333,  1343,     0,     0,     0,     0,     0,  1345,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,  1345,     0,     0,  1705,     0,     0,     0,
    1354,     0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,  1344,     0,     0,     0,  1345,
    1706,     0,     0,     0,  1354,     0,     0,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,  1345,     0,     0,  1337,     0,  1707,  1338,     0,     0,
    1354,     0,     0,  1708,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,  1332,     0,  1333,
       0,     0,  1354,     0,     0,     0,  1339,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,  1337,     0,     0,
    1338,     0,     0,  1340,     0,     0,     0,     0,     0,     0,
       0,  1341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1342,     0,     0,     0,     0,     0,  1339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1343,
       0,     0,     0,     0,  1345,  1337,  1340,     0,  1338,     0,
       0,     0,     0,     0,  1341,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,  1342,  1337,     0,     0,
    1338,  1709,     0,     0,     0,  1354,     0,  1339,     0,     0,
       0,     0,  1343,     0,     0,     0,     0,     0,     0,     0,
       0,  1344,     0,     0,  1340,     0,     0,     0,     0,  1339,
       0,  1337,  1341,     0,  1338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1342,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,     0,     0,     0,  1332,
    1343,  1333,     0,  1339,  1344,     0,  1342,     0,     0,     0,
    1334,  1335,  1336,     0,     0,     0,     0,     0,     0,     0,
    1340,  1332,  1343,  1333,     0,     0,     0,     0,  1341,     0,
       0,     0,  1334,  1335,  1336,     0,     0,     0,     0,     0,
    1342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1344,     0,     0,     0,  1343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1344,     0,     0,     0,  1337,     0,
    1345,  1338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,     0,     0,     0,     0,  1712,  1344,     0,
    1339,  1354,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1345,     0,     0,     0,  1340,     0,     0,
       0,     0,     0,     0,     0,  1341,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,  1332,  1342,  1333,     0,
    1713,     0,     0,     0,  1354,     0,     0,  1334,  1335,  1336,
       0,     0,     0,  1343,     0,     0,     0,     0,  1332,     0,
    1333,  1345,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1345,     0,     0,     0,     0,  1714,     0,
       0,     0,  1354,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,  1344,     0,     0,     0,     0,
    1715,     0,     0,     0,  1354,     0,     0,  1345,     0,     0,
    1337,     0,     0,  1338,     0,     0,     0,     0,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,  1337,     0,  1716,  1338,     0,  1332,  1354,  1333,
       0,     0,  1339,     0,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,     0,  1340,
       0,     0,     0,     0,  1339,     0,     0,  1341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1342,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,  1341,
       0,     0,  1332,     0,  1333,  1343,     0,     0,     0,     0,
       0,  1342,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,  1345,     0,     0,  1343,     0,     0,
       0,     0,     0,     0,     0,     0,  1717,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1354,     0,  1344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1337,     0,     0,
    1338,     0,     0,     0,     0,     0,     0,     0,     0,  1344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1337,
       0,     0,  1338,     0,     0,     0,     0,     0,     0,  1339,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1332,  1340,  1333,     0,     0,
       0,  1339,     0,     0,  1341,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,  1342,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,  1341,     0,     0,     0,
       0,     0,  1343,     0,     0,     0,     0,  1332,  1342,  1333,
       0,     0,     0,     0,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,  1343,     0,  1345,     0,  1337,     0,
       0,  1338,     0,     0,     0,     0,     0,     0,  1718,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,     0,
       0,     0,     0,     0,  1344,     0,     0,  1354,     0,     0,
    1339,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,     0,     0,  1719,  1344,  1340,     0,  1354,
       0,     0,     0,  1337,     0,  1341,  1338,     0,  1332,     0,
    1333,     0,     0,     0,     0,     0,     0,  1342,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
    1332,     0,  1333,  1343,     0,  1339,     0,     0,     0,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,     0,
       0,     0,  1340,  1332,     0,  1333,     0,     0,     0,     0,
    1341,     0,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1344,     0,     0,  1343,     0,
       0,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,  1345,  1337,     0,     0,  1338,
    1720,     0,     0,     0,  1354,     0,     0,  1722,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
    1344,     0,     0,     0,     0,     0,  1354,     0,  1339,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1337,     0,
       0,  1338,     0,     0,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,  1332,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,  1342,  1334,  1335,  1336,     0,
    1339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1343,     0,     0,  1345,     0,     0,  1340,     0,     0,
       0,     0,     0,     0,     0,  1341,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,  1342,     0,     0,
       0,  1723,     0,     0,     0,  1354,     0,     0,     0,  1337,
       0,     0,  1338,  1343,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1344,     0,     0,     0,     0,     0,  1345,
       0,  1337,     0,     0,  1338,     0,     0,     0,     0,     0,
       0,  1339,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,  1337,     0,  1726,  1338,  1340,     0,
    1354,     0,     0,  1339,     0,  1344,  1341,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1342,     0,
    1340,     0,     0,     0,     0,     0,  1339,     0,  1341,     0,
       0,     0,     0,     0,  1343,     0,     0,     0,     0,     0,
    1342,     0,  1332,  1340,  1333,     0,     0,     0,     0,     0,
       0,  1341,     0,  1334,  1335,  1336,  1343,     0,     0,     0,
       0,     0,     0,  1342,  1332,     0,  1333,     0,     0,     0,
       0,     0,     0,     0,     0,  1334,  1335,  1336,     0,  1343,
       0,     0,  1345,     0,     0,     0,  1344,     0,     0,     0,
       0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,  1344,  1727,
       0,     0,     0,  1354,     0,     0,  1337,     0,     0,  1338,
       0,     0,     0,     0,  1345,     0,     0,     0,     0,     0,
       0,  1344,     0,     0,     0,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,  1339,     0,
       0,  1728,     0,     0,     0,  1354,     0,     0,     0,     0,
       0,     0,     0,     0,  1332,  1340,  1333,     0,     0,     0,
       0,     0,     0,  1341,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,     0,  1342,  1332,     0,  1333,     0,
       0,     0,     0,     0,     0,     0,     0,  1334,  1335,  1336,
       0,  1343,     0,     0,     0,  1345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,     0,     0,
       0,     0,  1729,     0,     0,     0,  1354,     0,     0,  1730,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
    1345,     0,     0,  1344,     0,     0,     0,     0,  1354,     0,
       0,     0,  1731,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1337,     0,  1332,  1338,  1333,     0,     0,
       0,  1354,     0,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,  1337,     0,     0,  1338,     0,
       0,     0,     0,     0,     0,  1339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1340,     0,     0,     0,     0,  1339,     0,     0,
    1341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,  1340,     0,     0,     0,     0,     0,
       0,     0,  1341,     0,     0,     0,     0,     0,  1343,     0,
       0,     0,     0,     0,  1342,     0,     0,     0,     0,     0,
       0,     0,  1345,     0,     0,     0,     0,     0,     0,     0,
    1343,     0,     0,     0,  1733,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,  1337,     0,     0,  1338,     0,
       0,     0,     0,  1354,     0,     0,     0,     0,     0,     0,
    1344,     0,     0,     0,     0,     0,     0,  1337,     0,     0,
    1338,     0,  1332,     0,  1333,     0,     0,  1339,     0,     0,
       0,     0,  1344,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,  1340,  1332,     0,  1333,     0,  1339,
       0,     0,  1341,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,  1342,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,     0,     0,     0,     0,
    1343,     0,     0,     0,     0,  1332,  1342,  1333,     0,     0,
       0,     0,     0,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,  1343,     0,     0,     0,  1337,     0,  1332,  1338,
    1333,     0,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,  1345,
       0,     0,  1344,     0,     0,     0,     0,     0,  1339,     0,
       0,  1734,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,  1345,     0,     0,  1344,  1340,     0,     0,     0,     0,
    1354,     0,     0,  1341,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,  1342,     0,     0,  1739,     0,
       0,     0,  1354,     0,  1332,     0,  1333,     0,     0,     0,
       0,  1343,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1344,     0,     0,     0,     0,     0,     0,
       0,  1345,     0,  1337,     0,     0,  1338,     0,     0,     0,
       0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1345,     0,     0,  1337,     0,  1740,  1338,
       0,     0,  1354,     0,     0,  1339,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
    1741,     0,  1340,     0,  1354,     0,     0,     0,  1339,     0,
    1341,     0,     0,     0,     0,     0,  1337,     0,     0,  1338,
       0,     0,  1342,     0,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,     0,     0,     0,  1343,  1337,
       0,     0,  1338,     0,  1332,  1342,  1333,     0,  1339,     0,
       0,     0,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,  1343,  1345,     0,     0,  1340,     0,     0,     0,     0,
       0,  1339,     0,  1341,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,  1342,     0,     0,  1340,  1742,
    1344,     0,     0,  1354,     0,     0,  1341,     0,     0,     0,
       0,  1343,     0,     0,     0,     0,     0,     0,  1342,     0,
       0,     0,     0,  1344,     0,  1337,     0,  1332,  1338,  1333,
       0,     0,     0,     0,  1343,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1332,     0,  1333,     0,  1339,     0,     0,
       0,     0,     0,  1344,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,     0,  1340,     0,     0,  1332,     0,  1333,
       0,     0,  1341,     0,     0,     0,  1344,     0,  1334,  1335,
    1336,     0,     0,     0,  1342,     0,     0,     0,     0,     0,
       0,  1332,     0,  1333,     0,     0,     0,     0,     0,     0,
    1343,     0,  1334,  1335,  1336,     0,     0,     0,     0,  1345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,  1345,     0,     0,     0,  1743,     0,     0,     0,
    1354,     0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1344,  1353,     0,     0,     0,     0,     0,  1746,
       0,     0,     0,  1354,     0,     0,     0,     0,     0,     0,
       0,     0,  1345,     0,     0,  1337,     0,     0,  1338,     0,
       0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,  1345,     0,     0,     0,  1749,
       0,     0,     0,  1354,     0,     0,     0,  1339,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,  1750,     0,  1340,     0,  1354,     0,     0,     0,
       0,     0,  1341,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1342,     0,     0,     0,  1337,     0,
       0,  1338,     0,     0,     0,     0,     0,     0,     0,     0,
    1343,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1345,     0,     0,  1337,     0,     0,  1338,     0,     0,
    1339,     0,     0,  1751,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,  1340,  1337,     0,
       0,  1338,  1354,     0,     0,  1341,  1339,     0,     0,     0,
       0,     0,  1344,     0,     0,     0,     0,  1342,     0,     0,
       0,     0,  1337,  1340,  1332,  1338,  1333,     0,     0,     0,
    1339,  1341,     0,  1343,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,  1342,     0,     0,     0,  1340,     0,     0,
       0,     0,     0,     0,  1339,  1341,     0,     0,     0,  1343,
       0,     0,     0,     0,     0,     0,     0,  1342,     0,     0,
       0,  1340,     0,     0,     0,     0,     0,     0,     0,  1341,
       0,     0,  1332,  1343,  1333,  1344,     0,     0,     0,     0,
       0,  1342,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1343,  1332,     0,
    1333,  1344,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
    1332,  1345,  1333,     0,     0,  1344,     0,     0,     0,     0,
       0,  1334,  1335,  1336,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1332,     0,  1333,     0,     0,  1752,  1344,
       0,     0,  1354,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,     0,     0,  1332,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,     0,  1332,     0,  1333,
       0,     0,     0,     0,  1345,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
    1345,  1753,     0,     0,     0,  1354,     0,     0,     0,     0,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,     0,  1345,  1337,     0,  1754,  1338,     0,
       0,  1354,     0,     0,     0,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,  1345,     0,
       0,  1755,     0,     0,     0,  1354,     0,  1339,     0,     0,
       0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
       0,     0,     0,     0,  1340,  1756,     0,     0,     0,  1354,
       0,     0,  1341,  1337,     0,     0,  1338,     0,     0,     0,
       0,     0,     0,     0,  1342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1337,
    1343,     0,  1338,     0,     0,  1339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1337,  1340,     0,  1338,     0,     0,     0,     0,     0,
    1341,  1339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,  1337,     0,     0,  1338,  1340,     0,
       0,     0,  1344,  1339,     0,     0,  1341,     0,  1343,     0,
       0,     0,     0,     0,     0,     0,  1337,     0,  1342,  1338,
    1340,     0,     0,     0,     0,     0,  1339,     0,  1341,     0,
       0,     0,     0,     0,  1343,     0,     0,     0,  1337,     0,
    1342,  1338,  1332,  1340,  1333,     0,     0,     0,  1339,     0,
       0,  1341,     0,  1334,  1335,  1336,  1343,     0,     0,     0,
    1344,     0,     0,  1342,  1332,  1340,  1333,     0,     0,     0,
    1339,     0,     0,  1341,     0,  1334,  1335,  1336,     0,  1343,
       0,     0,     0,     0,     0,  1342,  1344,  1340,     0,     0,
       0,     0,     0,     0,     0,  1341,     0,     0,  1332,     0,
    1333,  1343,     0,     0,     0,     0,     0,  1342,  1344,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1345,     0,  1343,     0,     0,     0,     0,     0,     0,
       0,  1344,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1332,     0,  1333,     0,     0,  1757,     0,
       0,     0,  1354,  1344,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,     0,     0,  1332,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,  1344,  1334,  1335,  1336,  1345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,     0,  1345,  1758,     0,     0,     0,
    1354,     0,     0,     0,     0,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,     0,     0,
       0,     0,  1759,     0,     0,     0,  1354,     0,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
    1345,     0,     0,     0,  1760,     0,     0,     0,  1354,     0,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,  1345,  1337,     0,     0,  1338,  1761,     0,     0,
       0,  1354,     0,     0,  1762,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1345,  1337,     0,     0,  1338,     0,
       0,     0,     0,  1354,     0,  1339,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,  1765,  1340,     0,     0,  1354,     0,  1339,     0,  1337,
    1341,     0,  1338,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,  1340,     0,     0,     0,     0,     0,
       0,     0,  1341,     0,     0,     0,     0,     0,  1343,     0,
       0,  1339,     0,     0,  1342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1337,     0,     0,  1338,  1340,     0,
    1343,     0,     0,     0,     0,     0,  1341,     0,     0,     0,
       0,     0,  1332,     0,  1333,     0,  1337,     0,  1342,  1338,
       0,     0,     0,  1334,  1335,  1336,  1339,     0,     0,     0,
    1344,     0,     0,     0,  1343,     0,     0,     0,     0,     0,
       0,     0,  1332,  1340,  1333,     0,     0,     0,  1339,     0,
       0,  1341,  1344,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,  1342,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,     0,  1332,     0,  1333,  1343,
       0,     0,     0,     0,     0,  1342,  1344,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,     0,  1332,     0,
    1333,  1343,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1332,     0,  1333,     0,     0,     0,     0,     0,     0,
       0,  1344,  1334,  1335,  1336,     0,     0,     0,     0,     0,
       0,     0,     0,  1332,     0,  1333,     0,     0,     0,  1345,
       0,     0,     0,  1344,  1334,  1335,  1336,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,  1345,     0,     0,     0,     0,  1766,     0,     0,     0,
    1354,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,     0,  1767,     0,
       0,     0,  1354,     0,     0,  1345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,  1768,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,  1337,     0,     0,  1338,     0,     0,     0,
    1345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,  1345,  1337,     0,  1339,  1338,  1769,     0,     0,
       0,  1354,     0,     0,  1770,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1340,  1353,     0,     0,     0,     0,     0,     0,
    1341,     0,     0,  1354,     0,  1339,     0,  1337,     0,     0,
    1338,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1340,     0,     0,     0,     0,     0,  1343,  1337,
    1341,     0,  1338,     0,     0,     0,     0,     0,     0,  1339,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1337,     0,     0,  1338,  1340,     0,  1343,     0,
       0,  1339,     0,     0,  1341,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1337,     0,  1342,  1338,  1340,     0,
    1344,     0,     0,     0,  1339,     0,  1341,     0,     0,     0,
       0,     0,  1343,     0,     0,     0,     0,     0,  1342,     0,
    1332,  1340,  1333,     0,     0,     0,  1339,     0,     0,  1341,
    1344,  1334,  1335,  1336,  1343,     0,     0,     0,     0,     0,
       0,  1342,  1332,  1340,  1333,     0,     0,     0,     0,     0,
       0,  1341,     0,  1334,  1335,  1336,     0,  1343,     0,     0,
       0,     0,     0,  1342,  1344,  1332,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,     0,  1334,  1335,  1336,  1343,
       0,     0,     0,     0,     0,     0,  1344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1332,     0,  1333,     0,     0,     0,  1344,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,  1345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1344,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,  1332,     0,  1333,     0,     0,  1771,     0,     0,  1345,
    1354,     0,  1334,  1335,  1336,     0,     0,     0,     0,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,     0,     0,     0,  1772,     0,     0,     0,
    1354,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,  1345,     0,     0,     0,     0,
    1773,     0,     0,     0,  1354,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,  1345,     0,
       0,     0,  1774,     0,     0,     0,  1354,     0,     0,     0,
       0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
    1345,  1337,     0,     0,  1338,  1775,     0,     0,     0,  1354,
       0,     0,  1787,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1337,     0,     0,  1338,     0,     0,     0,
       0,  1354,     0,  1339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1337,     0,     0,  1338,
    1340,     0,     0,     0,     0,  1339,     0,     0,  1341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1342,     0,  1340,     0,     0,     0,     0,     0,  1339,     0,
    1341,     0,     0,     0,  1337,     0,  1343,  1338,     0,     0,
       0,     0,  1342,     0,     0,  1340,     0,     0,     0,     0,
    1332,     0,  1333,  1341,     0,     0,     0,     0,  1343,     0,
       0,  1334,  1335,  1336,     0,  1342,  1339,     0,     0,     0,
       0,     0,  1337,     0,     0,  1338,     0,     0,     0,     0,
       0,  1343,     0,  1340,     0,  1332,     0,  1333,  1344,     0,
       0,  1341,     0,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,  1342,  1339,     0,     0,     0,     0,     0,
    1344,     0,     0,     0,     0,     0,     0,     0,     0,  1343,
       0,  1340,     0,  1332,     0,  1333,     0,     0,     0,  1341,
       0,     0,     0,  1344,  1334,  1335,  1336,     0,     0,     0,
       0,  1342,     0,     0,     0,     0,     0,  1332,     0,  1333,
       0,     0,     0,     0,     0,     0,     0,  1343,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1344,     0,     0,  1332,     0,  1333,     0,     0,     0,
       0,     0,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1345,     0,  1344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1789,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,
       0,     0,     0,     0,     0,     0,  1794,     0,  1354,     0,
       0,  1792,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,  1345,     0,     0,     0,     0,     0,     0,     0,
    1354,     0,     0,     0,  1793,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,     0,     0,
       0,  1337,     0,  1354,  1338,     0,     0,     0,     0,     0,
    1345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1339,     0,     0,  1337,     0,     0,  1338,
       0,  1354,     0,     0,     0,     0,     0,     0,  1345,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,  1341,     0,
    1797,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1339,  1353,
    1342,     0,     0,     0,  1337,     0,     0,  1338,     0,  1354,
       0,     0,     0,     0,     0,  1340,  1343,     0,     0,     0,
       0,     0,     0,  1341,     0,     0,     0,     0,  1337,     0,
       0,  1338,     0,     0,     0,  1342,  1339,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1343,     0,  1340,     0,  1337,     0,     0,  1338,     0,
    1339,  1341,     0,     0,     0,     0,     0,     0,  1344,     0,
       0,     0,     0,  1342,     0,     0,     0,  1340,     0,     0,
       0,     0,  1332,     0,  1333,  1341,     0,  1339,     0,  1343,
       0,     0,     0,  1334,  1335,  1336,     0,  1342,     0,     0,
       0,     0,     0,  1344,  1340,  1332,     0,  1333,     0,     0,
       0,     0,  1341,  1343,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,  1342,     0,     0,     0,     0,     0,
       0,     0,  1332,     0,  1333,     0,     0,     0,     0,     0,
    1343,  1344,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1332,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,  1344,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,     0,     0,  1332,     0,  1333,
       0,     0,     0,     0,     0,     0,     0,  1345,  1334,  1335,
    1336,     0,  1344,     0,     0,     0,     0,     0,     0,  1798,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1354,     0,
       0,     0,  1345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1799,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1332,     0,  1333,     0,     0,     0,
       0,     0,     0,  1354,     0,  1334,  1335,  1336,     0,     0,
    1345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1800,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,     0,  1345,     0,     0,     0,     0,     0,
       0,  1354,     0,     0,     0,     0,  1802,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,  1345,     0,  1337,     0,  1354,  1338,     0,     0,     0,
       0,     0,     0,  1413,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,  1337,     0,     0,  1338,
       0,     0,  1354,     0,     0,  1339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1340,  1337,     0,     0,  1338,     0,  1339,     0,
    1341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1342,     0,     0,  1340,  1337,     0,     0,  1338,
       0,     0,     0,  1341,     0,  1339,     0,     0,  1343,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,  1337,     0,
       0,  1338,  1340,  1332,     0,  1333,     0,     0,  1339,     0,
    1341,  1343,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,  1342,     0,     0,  1340,     0,     0,     0,     0,
    1339,     0,     0,  1341,     0,     0,     0,     0,  1343,     0,
    1344,     0,     0,     0,     0,  1342,     0,  1340,     0,     0,
       0,     0,     0,     0,     0,  1341,     0,     0,     0,     0,
       0,  1343,     0,  1344,     0,  1337,     0,  1342,  1338,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1332,     0,  1333,  1343,     0,     0,     0,     0,     0,     0,
    1344,  1334,  1335,  1336,     0,     0,     0,  1339,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1344,  1340,  1332,     0,  1333,     0,     0,
       0,     0,  1341,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,  1342,  1344,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1332,     0,  1333,     0,     0,
    1343,     0,     0,     0,     0,     0,  1334,  1335,  1336,  1345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1803,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,  1345,     0,     0,     0,     0,     0,     0,     0,
    1354,     0,     0,     0,  1806,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1344,  1353,     0,     0,     0,     0,     0,  1345,
       0,     0,     0,  1354,     0,     0,     0,     0,     0,     0,
       0,  1819,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,  1345,     0,  1337,     0,     0,  1338,     0,     0,
    1354,     0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1345,  1855,     0,     0,     0,     0,
       0,     0,     0,  1354,     0,     0,  1339,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,  1857,     0,  1340,     0,  1354,     0,     0,     0,     0,
       0,  1341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1342,     0,     0,     0,     0,     0,     0,
       0,  1337,     0,     0,  1338,     0,     0,     0,     0,  1343,
       0,  1345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1339,     0,     0,  1337,     0,  1860,  1338,
       0,     0,  1354,     0,     0,     0,     0,     0,     0,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,  1341,     0,
       0,  1344,  1332,     0,  1333,     0,  1337,     0,  1339,  1338,
    1342,     0,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1340,  1343,     0,     0,     0,
       0,     0,     0,  1341,     0,  1332,     0,  1333,  1339,     0,
       0,     0,     0,     0,     0,  1342,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,  1340,     0,  1332,     0,  1333,
       0,  1343,     0,  1341,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,  1342,     0,     0,  1344,     0,
       0,     0,     0,  1332,     0,  1333,     0,     0,     0,     0,
       0,  1343,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,     0,     0,  1332,     0,  1333,     0,     0,
       0,     0,     0,  1344,     0,     0,  1334,  1335,  1336,     0,
    1345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1344,  1332,     0,  1333,  1867,     0,     0,
       0,  1354,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1332,     0,  1333,
       0,     0,     0,     0,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,  1345,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,     0,     0,  1870,     0,     0,     0,  1354,     0,
       0,     0,  1345,  1337,     0,     0,  1338,     0,     0,     0,
       0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,     0,  1871,
       0,     0,  1345,  1354,     0,  1339,  1337,     0,     0,  1338,
       0,     0,     0,     0,  1873,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1340,  1353,     0,     0,     0,     0,  1337,     0,
    1341,  1338,     0,  1354,     0,     0,     0,     0,  1339,     0,
       0,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1337,  1340,     0,  1338,  1343,     0,
    1339,     0,     0,  1341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1342,  1337,  1340,     0,  1338,
       0,     0,     0,     0,     0,  1341,  1339,     0,     0,     0,
       0,  1343,     0,     0,     0,     0,     0,  1342,     0,     0,
       0,     0,     0,  1340,     0,     0,     0,     0,  1339,     0,
    1344,  1341,     0,  1343,     0,  1337,     0,     0,  1338,     0,
       0,     0,     0,  1342,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,     0,     0,     0,     0,  1343,
       0,     0,     0,  1344,  1332,  1342,  1333,  1339,  1337,     0,
       0,  1338,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,  1343,     0,     0,  1340,  1344,  1332,     0,  1333,     0,
       0,     0,  1341,     0,     0,     0,     0,  1334,  1335,  1336,
    1339,     0,     0,     0,  1342,     0,     0,     0,     0,     0,
       0,  1344,     0,     0,     0,     0,     0,  1340,     0,  1332,
    1343,  1333,     0,     0,     0,  1341,     0,     0,     0,     0,
    1334,  1335,  1336,  1344,     0,     0,     0,  1342,     0,     0,
       0,     0,     0,     0,     0,  1332,     0,  1333,     0,  1345,
       0,     0,     0,  1343,     0,     0,  1334,  1335,  1336,     0,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,  1344,     0,     0,  1332,  1880,  1333,     0,     0,
    1354,     0,  1345,     0,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,  1881,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1345,  1344,     0,     0,     0,     0,
       0,     0,     0,  1354,     0,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
    1345,  1884,     0,     0,     0,  1354,     0,     0,     0,     0,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,  1345,     0,     0,     0,     0,  1885,     0,     0,
       0,  1354,     0,     0,  1886,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1354,     0,     0,     0,     0,     0,     0,
       0,  1345,     0,     0,     0,  1337,     0,     0,  1338,     0,
       0,     0,     0,  1887,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,  1337,     0,     0,
    1338,     0,  1354,     0,  1345,     0,     0,  1339,     0,     0,
       0,     0,     0,     0,     0,     0,  1888,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,  1340,  1353,     0,     0,     0,  1339,
    1337,     0,  1341,  1338,     0,  1354,     0,     0,     0,     0,
       0,     0,     0,     0,  1342,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,  1337,     0,     0,  1338,
    1343,     0,  1339,     0,     0,     0,  1342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1340,
       0,     0,  1343,     0,     0,     0,  1337,  1341,  1339,  1338,
    1332,     0,  1333,     0,     0,     0,     0,     0,     0,  1342,
       0,  1334,  1335,  1336,     0,  1340,     0,     0,     0,     0,
       0,     0,  1344,  1341,     0,  1343,     0,  1332,  1339,  1333,
       0,     0,     0,     0,     0,  1342,     0,     0,  1334,  1335,
    1336,     0,     0,     0,  1344,  1340,     0,     0,     0,     0,
    1332,  1343,  1333,  1341,     0,     0,     0,     0,     0,     0,
       0,  1334,  1335,  1336,     0,  1342,     0,     0,     0,     0,
       0,     0,  1332,     0,  1333,     0,     0,  1344,     0,     0,
       0,  1343,     0,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,     0,     0,  1332,     0,  1333,     0,     0,     0,
       0,     0,     0,  1344,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1332,     0,  1333,
       0,     0,     0,     0,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,  1344,     0,     0,     0,     0,     0,     0,
       0,  1345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,  1345,     0,     0,     0,     0,  1889,     0,
       0,     0,  1354,     0,     0,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
    1892,     0,     0,  1332,  1354,  1333,  1345,     0,     0,     0,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
       0,     0,  1345,  1893,     0,     0,     0,  1354,     0,     0,
       0,  1337,     0,     0,  1338,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,     0,  1894,
       0,     0,  1345,  1354,     0,     0,     0,     0,  1337,     0,
       0,  1338,     0,  1339,  1895,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,     0,     0,     0,
    1340,  1337,     0,  1354,  1338,     0,     0,     0,  1341,     0,
    1339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1342,     0,     0,  1337,     0,     0,  1338,  1340,     0,     0,
       0,     0,     0,  1339,     0,  1341,  1343,     0,     0,     0,
       0,     0,     0,     0,     0,  1337,     0,  1342,  1338,     0,
    1340,     0,     0,     0,     0,  1339,     0,     0,  1341,     0,
       0,     0,     0,  1343,     0,     0,     0,     0,  1337,     0,
    1342,  1338,  1340,     0,     0,     0,     0,  1339,     0,     0,
    1341,     0,     0,     0,     0,     0,  1343,     0,  1344,     0,
       0,     0,  1342,     0,  1340,     0,     0,     0,     0,     0,
    1339,     0,  1341,     0,     0,     0,     0,     0,  1343,     0,
       0,     0,     0,     0,  1342,  1344,     0,  1340,     0,     0,
       0,     0,  1332,     0,  1333,  1341,     0,     0,     0,     0,
    1343,     0,     0,  1334,  1335,  1336,     0,  1342,  1344,     0,
       0,     0,     0,     0,  1337,     0,  1332,  1338,  1333,     0,
       0,     0,     0,  1343,     0,     0,     0,  1334,  1335,  1336,
    1344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1332,     0,  1333,     0,  1339,     0,     0,     0,
       0,     0,  1344,  1334,  1335,  1336,     0,     0,     0,     0,
       0,     0,  1899,  1340,     0,     0,     0,     0,     0,     0,
       0,  1341,     0,     0,     0,  1344,     0,  1345,     0,     0,
       0,     0,     0,  1342,     0,     0,     0,     0,     0,  1897,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1343,
    1332,     0,  1333,     0,  1345,     0,     0,     0,  1354,     0,
       0,  1334,  1335,  1336,     0,     0,  1898,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,  1345,     0,     0,
       0,     0,     0,     0,     0,  1354,     0,     0,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,
       0,  1344,     0,     0,     0,     0,     0,     0,  1354,     0,
       0,  1901,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,  1345,     0,     0,     0,     0,     0,     0,     0,     0,
    1354,     0,     0,  1902,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,  1345,     0,     0,     0,     0,     0,
       0,     0,  1354,     0,     0,     0,  1903,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,     0,     0,  1337,     0,  1354,  1338,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1337,     0,     0,
    1338,  1332,     0,  1333,     0,  1339,     0,     0,     0,     0,
       0,     0,  1334,  1335,  1336,     0,     0,     0,     0,     0,
    1345,     0,  1340,  1337,     0,     0,  1338,     0,     0,  1339,
    1341,     0,  1904,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,  1342,     0,     0,     0,  1340,     0,     0,     0,
       0,  1354,     0,     0,  1341,  1339,     0,     0,  1343,     0,
       0,     0,     0,     0,     0,     0,  1342,     0,     0,     0,
       0,     0,  1340,     0,     0,     0,     0,  1332,     0,  1333,
    1341,  1337,  1343,     0,  1338,     0,     0,     0,  1334,  1335,
    1336,     0,  1342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1332,  1343,  1333,
    1344,     0,     0,  1339,     0,     0,     0,     0,  1334,  1335,
    1336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1340,     0,     0,     0,  1344,     0,     0,     0,  1341,     0,
    1332,     0,  1333,     0,     0,     0,     0,     0,     0,     0,
    1342,  1334,  1335,  1336,     0,     0,     0,     0,     0,     0,
    1344,     0,     0,  1332,     0,  1333,  1343,     0,     0,     0,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1332,     0,
    1333,     0,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,  1344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1345,
       0,     0,  1337,     0,     0,  1338,     0,     0,     0,     0,
       0,  1905,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
    1354,     0,     0,     0,  1339,     0,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,  1906,     0,     0,  1345,
       0,  1340,     0,     0,  1354,     0,     0,     0,     0,  1341,
       0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,
    1353,  1342,     0,     0,     0,     0,  1932,     0,  1337,     0,
    1354,  1338,     0,     0,     0,     0,     0,  1343,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1345,  1337,     0,
    1339,  1338,     0,     0,     0,     0,     0,     0,     0,     0,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1340,  1353,     0,
       0,     0,     0,     0,  1933,  1341,     0,     0,  1354,  1344,
    1339,  1337,     0,     0,  1338,     0,     0,  1342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1340,     0,     0,
       0,     0,     0,  1343,  1337,  1341,     0,  1338,     0,     0,
       0,     0,     0,  1339,     0,     0,     0,  1342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1340,     0,     0,  1343,     0,  1332,  1339,  1333,  1341,  1337,
       0,     0,  1338,     0,     0,     0,  1334,  1335,  1336,     0,
    1342,     0,     0,  1340,     0,  1344,     0,     0,     0,     0,
       0,  1341,     0,     0,     0,  1332,  1343,  1333,     0,     0,
       0,  1339,     0,  1342,     0,     0,  1334,  1335,  1336,     0,
       0,     0,     0,     0,     0,  1344,     0,     0,  1340,  1343,
       0,     0,     0,     0,     0,     0,  1341,     0,  1345,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1342,     0,
       0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1344,  1353,
       0,  1332,     0,  1333,  1343,  1934,     0,     0,     0,  1354,
       0,     0,  1334,  1335,  1336,     0,     0,     0,     0,     0,
       0,  1344,     0,  1332,     0,  1333,     0,     0,     0,     0,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1332,     0,  1333,     0,
       0,     0,     0,     0,  1345,     0,  1344,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
       0,  1935,     0,     0,  1345,  1354,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,
    1332,  1936,  1333,     0,     0,  1354,     0,  1345,     0,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,  1937,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
    1345,     0,     0,     0,     0,     0,  1337,     0,  1354,  1338,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,     0,     0,     0,     0,  1938,     0,     0,
       0,  1354,     0,     0,     0,  1345,  1337,     0,  1339,  1338,
       0,     0,     0,     0,     0,     0,     0,     0,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,  1340,  1353,     0,     0,     0,
       0,     0,  1941,  1341,     0,     0,  1354,     0,  1339,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1340,     0,     0,     0,     0,
       0,  1343,  1337,  1341,     0,  1338,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
       0,     0,     0,     0,  1337,     0,     0,  1338,     0,     0,
       0,  1343,     0,     0,  1339,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1337,     0,     0,
    1338,  1340,     0,  1344,     0,     0,  1339,     0,     0,  1341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1342,     0,  1340,     0,     0,     0,     0,  1332,  1339,
    1333,  1341,     0,  1344,     0,     0,     0,  1343,     0,  1334,
    1335,  1336,     0,  1342,     0,     0,  1340,     0,     0,     0,
       0,     0,     0,     0,  1341,     0,     0,     0,     0,  1343,
       0,  1337,     0,  1332,  1338,  1333,  1342,     0,     0,     0,
       0,     0,     0,     0,  1334,  1335,  1336,     0,     0,     0,
       0,     0,  1343,     0,     0,     0,     0,     0,     0,  1344,
    1332,     0,  1333,  1339,     0,     0,     0,     0,     0,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,     0,
    1340,  1344,     0,     0,     0,     0,     0,     0,  1341,     0,
       0,     0,  1345,     0,     0,     0,     0,     0,     0,     0,
    1342,     0,     0,     0,  1344,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,  1343,     0,     0,  1942,
       0,     0,  1345,  1354,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,  1332,     0,  1333,     0,     0,  1943,
       0,     0,     0,  1354,     0,  1334,  1335,  1336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1344,     0,
       0,     0,     0,     0,  1332,     0,  1333,     0,  1345,     0,
       0,     0,     0,     0,     0,  1334,  1335,  1336,     0,     0,
       0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,
    1345,     0,     0,     0,     0,  1946,     0,     0,     0,  1354,
       0,     0,     0,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,     0,  1345,     0,     0,     0,  1949,     0,  1337,
       0,  1354,  1338,     0,     0,  1950,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1354,     0,     0,     0,     0,     0,
       0,  1339,     0,     0,  1337,     0,     0,  1338,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,  1341,  1345,     0,     0,
       0,  1337,     0,     0,  1338,     0,  1339,     0,  1342,  1951,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,     0,  1340,  1343,     0,     0,     0,  1354,     0,
       0,  1341,     0,  1339,     0,  1332,     0,  1333,     0,     0,
       0,     0,     0,  1342,     0,     0,  1334,  1335,  1336,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,  1341,  1343,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1342,     0,     0,     0,     0,     0,  1344,     0,     0,     0,
       0,     0,     0,     0,     0,  1337,  1343,     0,  1338,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1344,     0,     0,     0,  1337,     0,  1339,  1338,  1332,
       0,  1333,     0,     0,     0,     0,     0,     0,     0,     0,
    1334,  1335,  1336,     0,  1340,     0,     0,     0,  1344,     0,
       0,     0,  1341,     0,     0,     0,  1332,  1339,  1333,     0,
       0,     0,     0,     0,  1342,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,  1340,     0,     0,     0,     0,     0,
    1343,     0,  1341,     0,     0,     0,     0,     0,     0,  1332,
       0,  1333,     0,     0,  1342,     0,     0,     0,     0,     0,
    1334,  1335,  1336,     0,     0,  1345,     0,     0,     0,     0,
    1343,     0,     0,     0,     0,     0,     0,  1952,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,  1344,     0,     0,     0,  1354,     0,     0,     0,
    1345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1953,  1346,  1347,  1348,  1349,  1350,  1351,  1352,
       0,  1353,  1344,     0,     0,     0,     0,  1345,     0,     0,
       0,  1354,     0,     0,     0,     0,  1337,     0,     0,  1338,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,     0,     0,  1964,     0,     0,     0,  1354,  1332,
       0,  1333,     0,     0,     0,     0,     0,     0,  1339,     0,
    1334,  1335,  1336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1340,     0,     0,     0,     0,
       0,     0,     0,  1341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1342,     0,     0,     0,     0,
       0,  1345,     0,     0,     0,     0,     0,     0,     0,     0,
    1337,  1343,     0,  1338,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,     0,  1965,     0,
       0,  1345,  1354,     0,     0,     0,     0,  1337,     0,     0,
    1338,     0,  1339,     0,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,     0,  1353,     0,     0,     0,     0,     0,  1966,  1340,
       0,     0,  1354,  1344,     0,     0,     0,  1341,     0,  1339,
    1337,     0,     0,  1338,     0,     0,     0,     0,     0,  1342,
       0,     0,     0,     0,     0,     0,  1340,     0,  1332,     0,
    1333,     0,     0,     0,  1341,  1343,     0,     0,     0,  1334,
    1335,  1336,  1339,     0,     0,     0,  1342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1332,  1340,
    1333,     0,  1343,     0,     0,     0,     0,  1341,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,  1342,
       0,     0,     0,     0,     0,     0,     0,  1344,     0,     0,
       0,     0,     0,     0,     0,  1343,     0,     0,  1332,     0,
    1333,     0,     0,     0,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,  1344,     0,     0,     0,     0,     0,
    1337,     0,  1345,  1338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1967,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,     0,  1353,     0,     0,     0,  1344,  1332,     0,
    1333,     0,  1339,  1354,     0,     0,     0,     0,     0,  1334,
    1335,  1336,     0,     0,     0,     0,     0,     0,     0,  1340,
    1332,     0,  1333,     0,     0,     0,     0,  1341,     0,     0,
       0,  1334,  1335,  1336,     0,     0,     0,     0,     0,  1342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1343,  1345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1968,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
       0,     0,     0,  1345,     0,     0,     0,  1354,     0,     0,
       0,     0,     0,     0,     0,  1969,  1346,  1347,  1348,  1349,
    1350,  1351,  1352,     0,  1353,     0,     0,  1344,     0,  1337,
       0,     0,  1338,     0,  1354,     0,  1345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,  1337,
       0,  1339,  1338,  1974,     0,     0,     0,  1354,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1340,     0,
       0,     0,     0,     0,     0,     0,  1341,     0,     0,     0,
       0,  1339,     0,     0,     0,     0,     0,     0,  1342,  1337,
       0,     0,  1338,     0,     0,     0,     0,     0,  1340,     0,
       0,     0,     0,     0,  1343,     0,  1341,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1342,     0,
       0,  1339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1343,     0,  1345,     0,  1340,  1337,
       0,     0,  1338,     0,     0,     0,  1341,     0,  1975,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,  1344,  1353,  1342,     0,
       0,  1337,     0,     0,  1338,     0,  1332,  1354,  1333,     0,
       0,  1339,     0,     0,  1343,     0,     0,  1334,  1335,  1336,
       0,     0,     0,     0,     0,     0,  1344,     0,  1340,  1332,
       0,  1333,     0,  1339,     0,     0,  1341,     0,     0,     0,
    1334,  1335,  1336,     0,     0,     0,     0,     0,  1342,     0,
    1340,     0,     0,     0,     0,     0,     0,     0,  1341,     0,
       0,     0,     0,     0,  1343,     0,  1344,     0,     0,     0,
    1342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1343,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1345,  1344,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1976,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,  1344,     0,
       0,     0,     0,     0,     0,  1345,  1354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1979,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,     0,     0,  1345,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1981,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1345,     0,  1337,     0,     0,
    1338,     0,     0,     0,     0,     0,     0,  1983,  1346,  1347,
    1348,  1349,  1350,  1351,  1352,     0,  1353,  1345,     0,     0,
    1337,     0,     0,     0,     0,     0,  1354,     0,     0,  1339,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,
       0,     0,     0,     0,     0,     0,  1340,     0,  1354,     0,
       0,     0,  1339,     0,  1341,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1342,     0,     0,  1340,
       0,     0,     0,     0,     0,     0,     0,  1341,     0,     0,
       0,     0,  1343,     0,     0,     0,     0,     0,     0,  1342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1345,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1346,  1347,  1348,  1570,
    1350,  1351,  1352,     0,  1353,     0,  1345,     0,     0,     0,
       0,     0,     0,     0,  1354,     0,     0,     0,     0,  1346,
    1347,  1348,  1349,  1350,  1351,  1352,     0,  1353,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1354
};

static const yytype_int16 yycheck[] =
{
       1,     2,     3,    70,  1382,   242,   160,     8,     9,    10,
      11,   158,    13,    63,    64,    65,     5,    18,    19,    20,
      21,    22,     5,     5,    25,    26,   173,   207,     5,   176,
      63,    64,    65,   507,   517,   516,   521,   505,    39,   522,
       0,   271,   499,   500,   518,   526,    47,    48,    49,    50,
      51,    52,    53,   521,   522,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,   296,    68,    69,   517,
      71,   523,    73,    74,   522,   505,   505,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    89,
       0,   521,   521,    94,    95,     5,   361,    98,   524,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   271,   134,   135,   136,  1360,     0,   139,   521,
     521,   142,     5,   144,   145,   505,   505,   517,   149,   150,
     151,     0,   522,     0,   155,   155,     5,   296,     5,     0,
     521,   521,   521,     0,     5,   521,   521,     0,     5,    79,
     521,   521,     0,   521,     5,     6,   521,     5,     9,   521,
     521,   521,  1406,   521,   204,   205,   521,   521,   521,     0,
      23,   521,     5,     6,     5,   521,     9,   521,   521,   521,
     521,   521,   521,   521,   521,   521,   521,   521,   228,   521,
      43,   521,     8,   521,   521,   521,    79,   521,   521,   521,
     521,     2,   521,    19,    20,    21,   521,   521,     9,   521,
      79,   521,    79,   521,    67,   521,   521,    70,    79,   521,
     521,   521,    79,   521,    75,   521,    20,    21,   521,   521,
     521,    79,   521,   521,   521,   521,   207,   521,   521,   521,
     521,   521,    75,    96,   521,   521,   521,   521,    79,   521,
      51,    52,    53,   521,   511,   521,   521,   521,   521,    60,
     521,   511,   512,   513,   514,   521,   516,   521,   521,   521,
     521,   507,   521,   521,   521,   521,   526,   521,   521,    80,
     521,   521,   521,   521,    85,   138,   521,   521,   141,   521,
     521,   521,   521,   521,    95,   521,   521,   521,   521,   521,
     321,   154,   521,   521,   521,   158,   521,   517,   522,   510,
     510,   517,   521,   521,   207,   522,    19,    20,    21,    22,
     173,    79,   361,   176,   522,   517,   127,   128,   507,   520,
     131,   132,   520,   517,   135,   522,    39,   522,   509,   522,
     522,   522,   207,    46,   522,   518,   522,    50,   207,   526,
     522,    54,    55,    56,   513,   514,   522,   516,    61,   522,
     522,   522,   522,   518,   522,   522,   522,   526,   522,    72,
      73,    74,   522,   522,   522,    78,    79,    80,   522,   522,
     522,   522,   522,    86,   522,    88,   522,   522,   520,   522,
     521,   525,   507,   522,   522,   525,   522,   517,   242,   518,
     518,   104,   936,  1326,   107,   522,  1568,   110,   900,  1895,
     113,   114,   801,   116,    67,   118,    67,   750,   121,   750,
     123,   461,   431,   126,    67,    10,   129,    -1,   431,   431,
      -1,   361,    -1,    -1,   431,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,  1847,
     471,    -1,    -1,    -1,    39,   271,   307,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,   505,    51,   488,    53,    54,
      55,    -1,    57,    -1,   307,    60,    -1,   271,   361,   519,
     296,   521,     2,    -1,    -1,    -1,    -1,    72,    -1,     9,
      -1,    -1,   361,    -1,   361,    80,    -1,    -1,    -1,    -1,
     361,   431,   296,    -1,   361,    -1,    -1,    -1,   517,    94,
      -1,    -1,   521,   361,   517,    -1,   537,    -1,   521,   522,
     694,    -1,   107,    -1,   109,   522,   377,   112,    -1,    -1,
     361,    51,    -1,    53,    -1,    -1,    -1,    -1,   712,    -1,
      -1,  1785,   127,   128,   377,    -1,    -1,   132,   431,    -1,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,   431,    -1,   431,    85,    -1,    -1,    -1,    -1,
     431,    -1,    -1,    -1,   431,    95,    -1,    -1,    -1,    -1,
     431,   432,    -1,   431,    -1,    -1,    -1,   517,    -1,    -1,
      -1,   521,    -1,   523,   524,    -1,    -1,    -1,   431,   432,
     431,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,   640,
      -1,   642,   643,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   517,    -1,   657,    -1,   521,    -1,
     523,   524,    -1,   664,    -1,    -1,    -1,   668,   517,    -1,
     671,    -1,   521,    -1,   523,   524,   523,   524,    -1,   510,
     681,    -1,   523,   524,   685,    -1,   523,   524,    -1,   690,
      -1,    -1,    -1,    -1,   695,   523,   524,   698,    -1,    -1,
      -1,    -1,   703,    -1,   517,    -1,   517,   488,    -1,    -1,
     521,   761,   523,   524,    -1,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,    -1,   726,    -1,    -1,   761,    -1,
     526,   732,    -1,    -1,    -1,    -1,    -1,   511,   512,   513,
     514,    -1,   516,    26,   745,    -1,    -1,    -1,    -1,   750,
      -1,    -1,   526,   754,    -1,    -1,    39,    -1,    -1,    -1,
     761,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      53,    -1,    -1,   927,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,   470,   471,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    80,    -1,    -1,
     801,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
     643,    -1,    95,    -1,    -1,   962,    -1,   818,   818,    -1,
      -1,    -1,    -1,    -1,   657,    -1,   827,    -1,   111,   112,
      -1,   832,    -1,    -1,    -1,   668,    -1,    -1,   671,    -1,
      -1,    -1,    -1,    -1,   127,   128,    -1,    -1,   681,   132,
      -1,    -1,   685,   854,    -1,    -1,    -1,   690,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   698,    -1,    -1,    -1,    -1,
     703,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   711,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   726,    -1,    -1,    -1,    -1,    -1,   732,
      -1,   902,    -1,    -1,    -1,   470,    -1,    -1,    -1,    -1,
     911,    -1,   745,    -1,    -1,    -1,    -1,   750,    -1,   920,
      -1,   754,    -1,    -1,    -1,    -1,    -1,    -1,   761,   930,
     930,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     941,   942,   943,   944,   945,    -1,   947,   948,    -1,    -1,
     951,   952,    -1,   954,   955,   956,   957,   958,    -1,   960,
     961,    -1,    -1,    -1,   965,   966,    -1,    -1,   801,   970,
     971,    -1,   973,    -1,    -1,    -1,   977,    -1,    -1,    -1,
     981,   982,    -1,    -1,    -1,   818,   987,   988,   989,   990,
      -1,    -1,    -1,    -1,   827,    -1,    -1,    -1,    -1,    -1,
      -1,  1002,  1003,  1004,  1005,    -1,  1007,  1008,  1009,  1010,
      -1,    -1,   695,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   854,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   902,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,   930,    -1,    -1,
      -1,    -1,    -1,   936,    -1,    -1,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1124,  1125,    -1,    -1,    -1,  1129,   962,
    1131,    -1,    -1,    -1,  1135,  1136,  1137,  1138,    -1,   972,
      -1,    -1,    -1,    -1,  1145,  1146,    -1,    -1,    -1,   832,
      -1,    -1,    -1,    -1,  1155,    -1,  1157,  1158,  1159,  1160,
     376,    -1,    -1,  1164,  1165,  1166,  1167,  1168,  1169,    -1,
    1171,    -1,    -1,  1327,    -1,    -1,  1177,  1178,    -1,  1180,
    1181,  1182,  1183,  1184,  1185,  1186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1199,    -1,
    1201,    -1,    -1,  1204,    -1,   488,    -1,    -1,    -1,    -1,
      -1,  1212,  1213,  1214,    -1,  1216,    -1,  1218,    -1,  1220,
      -1,  1222,    -1,    -1,  1225,  1226,  1227,  1228,  1229,    -1,
    1231,  1232,  1386,  1234,  1235,  1236,    -1,    -1,    -1,    -1,
      -1,  1242,    -1,  1244,  1245,  1246,  1247,    -1,    -1,  1250,
      -1,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,
    1261,    -1,    -1,  1264,  1265,    -1,  1267,  1268,  1269,  1270,
    1271,  1272,  1273,  1274,  1275,  1276,    -1,    -1,    -1,   495,
      -1,    -1,    -1,  1284,  1285,    -1,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,    -1,    -1,   522,  1308,    -1,    -1,
     526,  1312,  1312,  1146,    -1,    -1,    -1,  1384,    -1,    -1,
      -1,  1388,    -1,  1324,  1325,    -1,    -1,    -1,    -1,    -1,
      -1,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,
    1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,    -1,    -1,    -1,  1358,    -1,  1360,
      -1,    -1,    -1,  1364,    -1,    -1,    -1,  1368,    -1,  1370,
    1371,  1372,    -1,    -1,  1375,    -1,    -1,  1378,    -1,  1380,
    1381,     0,    -1,    -1,    -1,    -1,     5,    -1,    -1,    -1,
    1391,    -1,  1393,    -1,  1395,    -1,    -1,    -1,  1399,    -1,
    1401,    -1,    -1,    -1,    23,  1406,    -1,    -1,  1409,    -1,
      -1,  1412,  1413,  1413,  1415,    -1,    -1,  1418,    -1,    -1,
    1421,    40,    41,    42,    43,    44,    45,  1428,    -1,    -1,
       0,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    -1,
      -1,    70,  1453,    23,    -1,    -1,    75,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,  1165,  1484,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,  1179,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,   138,
      -1,    -1,   141,    -1,    -1,    -1,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1230,  1231,    -1,
      -1,  1384,    -1,    -1,   173,  1388,    -1,   176,    -1,    -1,
      -1,  1244,    -1,  1246,    -1,    -1,    -1,  1568,   138,  1570,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,  1160,    -1,    -1,    -1,    -1,
    1165,    -1,    -1,   173,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1179,    -1,  1617,  1618,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1308,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1648,  1649,    -1,
      -1,    -1,  1653,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1227,    -1,  1665,  1230,    -1,  1668,  1669,  1670,
      -1,  1672,    -1,  1674,    -1,  1676,    -1,  1678,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1690,
    1691,    -1,  1693,    -1,    -1,    -1,  1697,   316,    -1,    -1,
      -1,    -1,  1703,  1704,    -1,    -1,    -1,  1708,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1717,  1718,    -1,    -1,
      -1,  1722,    -1,  1724,    -1,    -1,    -1,    -1,    -1,  1730,
    1731,    -1,  1733,  1734,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   316,    -1,    -1,    -1,
    1751,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1762,  1763,    -1,    -1,    -1,    -1,    -1,    -1,  1770,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1779,    -1,
      -1,    -1,    -1,    -1,  1785,    -1,  1787,    -1,  1789,    -1,
      -1,  1792,  1793,  1794,    -1,    -1,  1797,  1798,  1799,  1800,
       8,  1802,  1803,    -1,    -1,  1806,    -1,  1808,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,  1819,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1858,    -1,    -1,
    1861,    -1,  1863,    -1,  1865,    -1,    -1,  1868,    -1,    -1,
      -1,    -1,  1873,  1874,    -1,  1876,    -1,  1878,    -1,    -1,
    1881,  1882,    -1,    -1,    -1,  1886,  1887,  1888,    -1,  1890,
      -1,    -1,    -1,    -1,  1895,    -1,  1897,  1898,  1899,    -1,
    1901,  1902,  1903,  1904,  1905,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    -1,    -1,    -1,  1937,    -1,  1939,    -1,
      19,    20,    21,  1944,    -1,    -1,  1947,    -1,    -1,  1950,
    1951,  1952,  1953,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1962,    -1,    -1,    -1,    -1,  1967,  1968,  1969,    -1,
     589,    -1,    -1,    -1,  1975,  1976,    -1,    -1,  1979,     8,
    1981,    10,  1983,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   589,
      -1,    -1,    -1,    -1,   643,    -1,    -1,    -1,    -1,    -1,
      -1,   239,    -1,    -1,    -1,    -1,    -1,    -1,   657,    -1,
      -1,    -1,    -1,   662,    -1,    -1,    -1,    -1,    -1,   668,
      -1,    -1,   671,   239,    -1,    -1,   242,     8,    -1,    10,
      -1,    -1,   681,   271,    -1,    -1,   685,    -1,    19,    20,
      21,   690,    -1,   643,    -1,    -1,    -1,    -1,    -1,   698,
     288,    -1,    -1,    -1,   703,   271,    -1,   657,   296,    -1,
      -1,    -1,   711,    -1,    -1,    -1,    -1,    -1,   668,    -1,
     308,   671,   288,    -1,    -1,    -1,    -1,   726,    -1,    -1,
     296,   681,    -1,   732,    -1,   685,    -1,    -1,    -1,    -1,
     690,    -1,   308,    -1,    -1,    -1,   745,    -1,   698,    -1,
      -1,   750,    -1,   703,    -1,   754,    -1,   239,   324,    -1,
     242,    -1,   761,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   726,    -1,    -1,    -1,
     239,    -1,   732,   242,    -1,    -1,    -1,    -1,    -1,   271,
      -1,    -1,    -1,    -1,    -1,   745,    -1,    -1,   207,    -1,
     750,    -1,   801,    -1,   754,    -1,   288,    -1,    -1,    -1,
     376,   761,   271,    -1,   296,    -1,    -1,    -1,    -1,   818,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,   827,   288,
     239,    -1,    -1,   242,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,   801,    -1,    -1,    -1,   854,    -1,    -1,     8,    -1,
      10,    -1,   271,    -1,    -1,   324,    -1,    -1,   818,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,   827,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,   902,   854,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,    -1,    -1,   324,    -1,   376,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,   495,
      -1,   930,    -1,    -1,    -1,    -1,    -1,   936,   526,    -1,
     271,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,   902,    -1,    -1,    -1,   522,   288,    -1,    -1,
     526,    -1,    -1,   962,    -1,   296,    -1,   376,   967,    -1,
      -1,    -1,    -1,   972,    -1,    -1,    -1,   308,    -1,    -1,
     930,    -1,    -1,    -1,    -1,    -1,   936,    -1,    -1,    -1,
      -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   962,   495,    -1,    -1,    -1,   967,    -1,    -1,
      -1,    -1,   972,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,   495,    -1,    -1,    -1,
     522,    -1,    -1,    -1,   526,   376,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
      -1,    -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,
      -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,   288,    18,
      19,    -1,    -1,    -1,    -1,    -1,   296,  1146,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    38,
      -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   324,    -1,   507,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,   522,    71,    -1,    -1,   526,  1146,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    -1,    -1,    -1,    97,    -1,
      -1,   100,   101,    -1,    -1,    -1,   376,    -1,    -1,   108,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,   144,    -1,   146,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,  1382,    -1,  1384,    -1,    -1,    -1,  1388,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1405,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   322,   323,   324,   325,   326,    -1,    -1,
     329,   330,   331,   332,    -1,  1405,    -1,    -1,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,    -1,
      -1,    -1,   351,    -1,    -1,    -1,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,    -1,
      -1,    -1,    -1,    -1,   473,   474,   475,   476,    -1,    -1,
      -1,    -1,   481,   482,   483,   484,   485,   486,    -1,    -1,
     489,   490,   491,   492,   493,   494,    -1,    -1,    -1,    -1,
     499,   500,   501,    -1,    -1,    -1,  1625,    -1,   507,   508,
     509,   510,   511,   512,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,    -1,    -1,
     539,   540,   541,   542,   543,   544,   545,    -1,    -1,    -1,
     549,    -1,   551,   552,   553,   554,   555,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   564,    -1,    -1,   567,   568,
     569,   570,   571,   572,    -1,    -1,    -1,   576,   577,   578,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   591,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   602,   603,   604,   605,   606,   607,   608,
      -1,    -1,   611,   612,   613,   614,   615,   616,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   627,   628,
     629,   630,   631,   632,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    18,    -1,    -1,  1783,    22,    23,    24,    25,    26,
      -1,    -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,
      37,    -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    -1,    -1,    64,    65,    66,
      -1,    -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1847,    -1,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,
      -1,   208,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,
     237,   238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,
      -1,    -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,
      -1,   258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,
      -1,    -1,   269,   270,    -1,   272,   273,   274,    -1,   276,
     277,   278,   279,   280,   281,    -1,    -1,   284,   285,   286,
     287,    -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
      -1,    -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,
     317,    -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,
     327,   328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,
     337,    -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,
     347,   348,    -1,   350,   351,   352,   353,   354,    -1,   356,
      -1,    -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,
     367,   368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,
      -1,   378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,
     387,   388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,
      -1,    -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,
      -1,    -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,
     417,    -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,
     427,   428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,
     437,   438,    -1,    -1,    -1,   442,   443,   444,   445,   446,
     447,   448,   449,    -1,   451,   452,   453,   454,   455,   456,
     457,   458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,    -1,    -1,   485,    -1,
     487,    -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,    -1,
     507,    -1,    -1,    -1,   511,   512,    -1,    -1,    -1,    -1,
      -1,   518,   519,    -1,   521,     3,     4,     5,   525,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
     358,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,   522,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
     358,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,   522,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
     358,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,   522,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,   522,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,   522,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,   522,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,   207,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,   510,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
     518,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,   509,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
     358,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,    -1,    -1,
      -1,    -1,    -1,   511,   512,     3,     4,     5,    -1,    -1,
      -1,   519,    -1,   521,    -1,    13,    14,    15,    16,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    30,    -1,    -1,    33,    -1,    35,    -1,    37,
      -1,    39,    40,    -1,    -1,    43,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    -1,
      -1,    69,    70,    71,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,    -1,    -1,    -1,   203,   204,   205,    -1,    -1,
     208,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,    -1,    -1,   232,    -1,    -1,    -1,   236,   237,
     238,    -1,   240,    -1,    -1,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,    -1,    -1,    -1,
     258,   259,   260,   261,    -1,    -1,   264,    -1,    -1,    -1,
      -1,   269,   270,    -1,   272,   273,   274,    -1,   276,   277,
     278,   279,   280,   281,    -1,    -1,   284,   285,   286,   287,
      -1,   289,    -1,    -1,   292,   293,   294,   295,    -1,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,    -1,   317,
      -1,    -1,    -1,    -1,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,    -1,    -1,    -1,    -1,   335,    -1,   337,
      -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,   350,   351,   352,   353,   354,    -1,   356,    -1,
      -1,    -1,    -1,    -1,   362,   363,   364,    -1,    -1,   367,
     368,   369,   370,   371,    -1,   373,    -1,    -1,    -1,    -1,
     378,    -1,    -1,    -1,    -1,    -1,   384,   385,   386,   387,
     388,    -1,    -1,    -1,    -1,   393,   394,   395,   396,    -1,
      -1,    -1,   400,    -1,   402,   403,    -1,   405,    -1,    -1,
      -1,    -1,    -1,   411,    -1,   413,    -1,    -1,    -1,   417,
      -1,    -1,   420,   421,    -1,    -1,   424,   425,   426,   427,
     428,    -1,    -1,   431,    -1,    -1,    -1,   435,    -1,   437,
     438,    -1,    -1,    -1,   442,   443,   444,   445,   446,   447,
     448,   449,    -1,   451,   452,   453,   454,   455,   456,   457,
     458,    -1,    -1,   461,    -1,   463,    -1,   465,    -1,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,    -1,    -1,   485,    -1,   487,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    -1,    17,    -1,
      -1,   519,    -1,   521,    -1,    -1,    -1,    -1,    27,    28,
      29,    -1,    31,    32,    -1,    34,    -1,    36,    -1,    38,
      -1,    -1,    41,    42,    -1,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,    -1,    -1,   206,    -1,    -1,
     209,   210,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     229,   230,   231,    -1,   233,   234,   235,    -1,    -1,    -1,
      -1,    -1,   241,    -1,   243,   244,    -1,   246,   247,   248,
      -1,    -1,    -1,    -1,   253,   254,   255,   256,   257,    -1,
      -1,    -1,    -1,   262,   263,    -1,   265,   266,   267,   268,
      -1,    -1,    -1,    -1,    -1,    -1,   275,    -1,    -1,    -1,
      -1,    -1,    -1,   282,   283,    -1,    -1,    -1,    -1,    -1,
      -1,   290,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,   311,    -1,   313,    -1,   315,   316,    -1,   318,
     319,   320,   321,    -1,   323,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   331,   332,   333,   334,    -1,   336,    -1,   338,
     339,    -1,   341,   342,   343,   344,   345,   346,    -1,    -1,
     349,    -1,    -1,    -1,    -1,    -1,   355,   239,   357,    -1,
     359,   360,   361,    -1,    -1,    -1,   365,   366,    -1,    -1,
      -1,    -1,    -1,   372,    -1,   374,   375,    -1,    -1,    -1,
     379,   380,   381,   382,   383,    -1,    -1,    -1,    -1,   271,
     389,   390,   391,   392,    -1,    -1,    -1,    -1,   397,   398,
     399,   239,   401,    -1,   242,   404,   288,   406,   407,   408,
     409,   410,    -1,   412,   296,   414,   415,   416,    -1,   418,
     419,    -1,    -1,   422,   423,    -1,   308,    -1,    -1,    -1,
     429,    -1,    -1,   271,   433,   434,    -1,   436,    -1,    -1,
      -1,   440,   324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,   450,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
     459,   460,    -1,   462,    -1,   464,    -1,   466,    -1,    -1,
     308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   483,   484,   324,   486,    -1,   488,
     489,   490,    -1,   492,   493,   494,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   505,   506,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,   521,    -1,    -1,    -1,   525,    -1,    27,    28,
      29,    -1,    31,    32,    -1,    34,    -1,    36,   376,    38,
      -1,    -1,    41,    42,    -1,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    63,    -1,    -1,    -1,    67,    68,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,   201,   202,    -1,    -1,    -1,   206,    -1,    -1,
     209,   210,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     229,   230,   231,    -1,   233,   234,   235,    -1,    -1,    -1,
      -1,    -1,   241,    -1,   243,   244,    -1,   246,   247,   248,
      -1,    -1,    -1,    -1,   253,   254,   255,   256,   257,    -1,
      -1,    -1,    -1,   262,   263,    -1,   265,   266,   267,   268,
      -1,    -1,    -1,    -1,    -1,    -1,   275,    -1,    -1,    -1,
      -1,    -1,    -1,   282,   283,    -1,    -1,    -1,    -1,    -1,
      -1,   290,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   310,   311,    -1,   313,    -1,   315,   316,    -1,   318,
     319,   320,   321,    -1,   323,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   331,   332,   333,   334,    -1,   336,    -1,   338,
     339,    -1,   341,   342,   343,   344,   345,   346,    -1,    -1,
     349,    -1,    -1,    -1,    -1,    -1,   355,    -1,   357,    -1,
     359,   360,   361,    -1,    -1,    -1,   365,   366,    -1,    -1,
      -1,    -1,    -1,   372,    -1,   374,   375,    -1,    -1,    -1,
     379,   380,   381,   382,   383,    -1,    -1,    -1,    -1,    -1,
     389,   390,   391,   392,    -1,    -1,    -1,    -1,   397,   398,
     399,    -1,   401,    -1,    -1,   404,    -1,   406,   407,   408,
     409,   410,    -1,   412,    -1,   414,   415,   416,    -1,   418,
     419,    -1,    -1,   422,   423,    -1,    -1,    -1,    -1,    -1,
     429,    -1,    -1,    -1,   433,   434,    -1,   436,    -1,    -1,
      -1,   440,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   450,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     459,   460,    -1,   462,    -1,   464,    -1,   466,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,
      11,    12,    -1,    -1,   483,   484,    17,   486,    -1,   488,
     489,   490,    -1,   492,   493,   494,    27,    28,    29,    -1,
      31,    32,    -1,    34,    -1,    36,   505,    38,    -1,    -1,
      41,    42,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,   521,    -1,    -1,    -1,   525,    -1,    -1,    -1,
      61,    62,    63,    -1,    -1,    -1,    67,    68,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
     201,   202,    -1,    -1,    -1,   206,    -1,    -1,   209,   210,
     211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   229,   230,
     231,    -1,   233,   234,   235,    -1,    -1,    -1,    -1,    -1,
     241,    -1,   243,   244,    -1,   246,   247,   248,    -1,    -1,
      -1,    -1,   253,   254,   255,   256,   257,    -1,    -1,    -1,
      -1,   262,   263,    -1,   265,   266,   267,   268,    -1,    -1,
      -1,    -1,    -1,    -1,   275,     8,    -1,    10,    -1,    -1,
      -1,   282,   283,    -1,    -1,    -1,    19,    20,    21,   290,
     291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   310,
     311,    -1,   313,    -1,   315,   316,    -1,   318,   319,   320,
     321,    -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     331,   332,   333,   334,    -1,   336,    -1,   338,   339,    -1,
     341,   342,   343,   344,   345,   346,    -1,    -1,   349,     8,
      -1,    10,    -1,    -1,   355,    -1,   357,    -1,   359,   360,
      19,    20,    21,    -1,   365,   366,    -1,    -1,    -1,    -1,
      -1,   372,    -1,   374,   375,    -1,    -1,    -1,   379,   380,
     381,   382,   383,    -1,    -1,    -1,    -1,    -1,   389,   390,
     391,   392,    -1,    -1,    -1,    -1,   397,   398,   399,   239,
     401,    -1,   242,   404,    -1,   406,   407,   408,   409,   410,
      -1,   412,    -1,   414,   415,   416,    -1,   418,   419,    -1,
      -1,   422,   423,    -1,    -1,    -1,    -1,    -1,   429,    -1,
      -1,   271,   433,   434,    -1,   436,    -1,    -1,    -1,   440,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,   450,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,   459,   460,
      -1,   462,    -1,   464,    -1,   466,    -1,    -1,   308,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   483,   484,   324,   486,    -1,   488,   489,   490,
      -1,   492,   493,   494,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   505,    -1,   239,    -1,    -1,   242,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     521,    19,    20,    21,   525,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,   271,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,
     239,    -1,     8,   242,    10,    -1,    -1,    -1,    -1,    -1,
      -1,   324,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,   271,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,
       8,    -1,    10,   376,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    19,    20,    21,    -1,   495,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   324,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,     8,    -1,
      10,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,   495,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,   271,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,
     288,    -1,    -1,   526,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     308,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   495,   239,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   271,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
      -1,    -1,   288,   522,    -1,    -1,    -1,   526,    -1,   271,
     296,   239,    -1,    -1,   242,    -1,    -1,    -1,   376,    -1,
      -1,    -1,   308,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,   324,    -1,
      -1,    -1,    -1,   271,    -1,    -1,   308,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,   324,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
     308,   271,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
     376,    -1,    -1,   207,    -1,    -1,   324,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,   308,    -1,
      -1,    -1,    -1,    -1,    -1,   239,    -1,     8,   242,    10,
      -1,    -1,    -1,    -1,   324,    -1,    -1,   495,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,   507,
     508,   509,   510,   511,   512,   513,   514,   271,   516,    -1,
      -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   296,    -1,    -1,     8,   376,    10,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,
     324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,   495,    -1,    -1,   522,    -1,    -1,    -1,
     526,    -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
     522,    -1,   376,    -1,   526,    -1,    -1,   495,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,     8,
      -1,    10,    -1,   239,    -1,   495,   242,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   522,    -1,    -1,   271,   526,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,     8,    -1,    10,    -1,   239,    -1,
     296,   242,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,
     271,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,   526,    -1,    -1,   296,   239,    -1,     8,   242,
      10,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,   324,     8,    -1,    10,    -1,   271,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,     8,    -1,    10,    -1,
      -1,    -1,    -1,   296,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,
      -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,   495,
      -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,   288,
     516,    -1,    -1,    -1,    -1,   239,   522,   296,   242,     8,
     526,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,   495,   324,    -1,   271,    -1,    -1,
      -1,    -1,    19,    20,    21,    -1,   507,   508,   509,   510,
     511,   512,   513,   514,   288,   516,    -1,   518,    -1,    -1,
      -1,    -1,   296,    -1,    -1,   526,    -1,    -1,    -1,   239,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,
     324,    -1,   495,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,   271,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,   239,   288,   522,
     242,    -1,    -1,   526,    -1,    -1,   296,   271,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
      -1,    -1,   376,    -1,   288,    -1,    -1,    -1,    -1,   271,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,   239,
      -1,    -1,   242,    -1,   308,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,     8,    -1,    10,    -1,
     324,    -1,    -1,    -1,    -1,    -1,   308,    19,    20,    21,
      -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,   495,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,   308,    -1,
      -1,    -1,   376,   522,    -1,    -1,    -1,   526,    -1,    -1,
     239,    -1,    -1,   242,   324,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,
      -1,   495,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,    -1,   271,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,   288,
      -1,     8,   526,    10,   271,    -1,   376,   296,    -1,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,   308,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,     8,    -1,    10,    -1,   324,    -1,    -1,    -1,    -1,
      -1,   308,    19,    20,    21,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,   324,    -1,    -1,
      -1,   495,    -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,   495,    -1,    -1,    -1,   376,   522,    -1,
      -1,    -1,   526,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,   376,
     522,    -1,    -1,    -1,   526,    -1,    -1,   239,    -1,    -1,
     242,     8,    -1,    10,    -1,   495,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,   271,
      -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,   324,    -1,    -1,    -1,   495,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,   495,    -1,
      -1,    -1,   239,   522,    -1,   242,    -1,   526,    -1,    -1,
     507,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,    -1,   376,   522,    -1,    -1,    -1,   526,
      -1,    -1,   239,    -1,   271,   242,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,     8,    -1,    10,   271,    -1,    -1,    -1,    -1,    -1,
      -1,   308,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,   324,    -1,   296,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,    -1,
      -1,    -1,   239,    -1,     8,   242,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,   376,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   271,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,   376,
     522,   288,    -1,    -1,   526,    -1,    -1,   239,    -1,   296,
     242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
      -1,    -1,   242,     8,    -1,    10,    -1,   324,    -1,   271,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,   288,    10,    -1,    -1,
      -1,   271,    -1,    -1,   296,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,   495,   376,
      -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,   308,    -1,
     507,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,    -1,   324,   522,    -1,    -1,   495,   526,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
     507,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,    -1,   376,   522,    -1,    -1,     8,   526,
      10,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,     8,    -1,    10,    -1,   239,    -1,    -1,   242,    -1,
      -1,   308,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,   324,   495,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,   271,    -1,    -1,
     507,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,    -1,   288,   522,    -1,    -1,    -1,   526,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,   376,
      -1,    -1,    -1,   495,   239,    -1,    -1,   242,    -1,    -1,
     324,    -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,   495,   239,    -1,    -1,   242,
     522,    -1,    -1,    -1,   526,    -1,   271,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   522,   288,    -1,    -1,   526,    -1,   271,    -1,
      -1,   296,   376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,     8,    -1,    10,    -1,   324,
      -1,    -1,    -1,    -1,    -1,   308,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   324,    -1,    -1,    -1,     8,    -1,    10,   495,   239,
      -1,    -1,   242,    -1,    -1,    -1,    19,    20,    21,    -1,
     507,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,   376,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,
      -1,   271,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,   239,    -1,   296,   242,     8,    -1,
      10,   495,    -1,    -1,   271,    -1,    -1,    -1,   308,    19,
      20,    21,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,   288,   516,    -1,   324,    -1,   271,    -1,   522,   296,
      -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   308,    -1,   288,    -1,    -1,    -1,    -1,     8,    -1,
      10,   296,    -1,    -1,    -1,    -1,    -1,   324,    -1,    19,
      20,    21,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,   324,
     495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,   495,    -1,    -1,    -1,    -1,   522,    -1,   376,
      -1,   526,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,
      -1,   376,    -1,   526,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,   239,    -1,   271,   242,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,   495,    -1,    -1,   271,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,   288,   516,    -1,    -1,    -1,
      -1,   324,   522,   296,    -1,     8,   526,    10,   495,   239,
      -1,    -1,   242,    -1,    -1,   308,    19,    20,    21,    -1,
     507,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
     495,   324,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,
      -1,   271,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   376,    -1,    -1,    -1,   522,   288,   239,
      -1,   526,   242,     8,    -1,    10,   296,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,   308,    -1,
      -1,    -1,    -1,   376,    -1,    -1,     8,    -1,    10,    -1,
      -1,   271,    -1,    -1,   324,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   324,    -1,   376,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,   495,   239,    -1,    -1,   242,    -1,    -1,    -1,
      19,    20,    21,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,   376,    -1,    -1,   522,
      -1,    -1,   495,   526,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,   288,   516,    -1,    -1,    -1,    -1,    -1,   522,
     296,    -1,     8,   526,    10,    -1,   239,    -1,    -1,   242,
      -1,    -1,   308,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,   271,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,   288,   516,    -1,    -1,    -1,
      -1,    -1,   522,   296,   239,    -1,   526,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,
     376,    -1,    -1,    -1,    -1,   495,    -1,   239,    -1,    -1,
     242,   324,    -1,    -1,    -1,    -1,   271,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   522,   288,    -1,    -1,   526,    -1,    -1,   271,
     239,   296,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   308,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,   376,   296,    -1,    -1,    -1,     8,   324,
      10,    -1,   271,    -1,    -1,    -1,   308,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
     239,    -1,   324,   242,     8,    -1,    10,   296,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,   308,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,   495,
      -1,   376,   271,    -1,    -1,   324,    -1,    19,    20,    21,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,   288,
     516,    -1,    -1,    -1,   376,    -1,   522,   296,    -1,     8,
     526,    10,    -1,   239,    -1,    -1,   242,    -1,    -1,   308,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   324,    -1,   376,    -1,    -1,
      -1,    -1,   495,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,   288,   516,    -1,    -1,    -1,    -1,    -1,   522,
     296,    -1,     8,   526,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    19,    20,    21,    -1,   376,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,
     495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   495,    -1,    -1,    -1,   522,    -1,    -1,
      -1,   526,    -1,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,    -1,   526,    -1,   495,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,    -1,   526,   242,    -1,
      -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   495,   239,   288,    -1,
     242,    -1,    -1,    -1,    -1,    -1,   296,   271,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,   308,    -1,
      -1,    -1,    -1,    -1,   288,    -1,     8,   526,    10,   271,
     239,    -1,   296,   242,   324,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,   308,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,     8,    -1,    10,   495,
     324,    -1,   271,    -1,    -1,    -1,   308,    19,    20,    21,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,   288,
     516,    -1,   324,    -1,    -1,    -1,   376,   296,    -1,     8,
     526,    10,    -1,   239,    -1,    -1,   242,    -1,    -1,   308,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,   324,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,   271,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,   376,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   439,    -1,     8,
      -1,    10,    -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
     376,   495,    -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,   495,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,   526,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,   526,    10,   495,   239,    -1,   271,
     242,    -1,    -1,    -1,    19,    20,    21,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,   288,   516,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,    -1,   526,    -1,   271,
     239,    -1,    -1,   242,    -1,    -1,   308,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,   324,    -1,   296,    -1,    -1,   239,    -1,   495,
     242,    -1,   271,    -1,    -1,    -1,   308,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,   288,
     516,    -1,   324,    -1,    -1,    -1,    -1,   296,    -1,   271,
     526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,   376,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,   239,   296,   324,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   376,    -1,     8,    -1,    10,    -1,
     239,    -1,   324,   242,    -1,   271,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,     8,    -1,    10,    -1,   376,    -1,    -1,
     296,    -1,   271,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,   376,    -1,    -1,   296,   324,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   495,   239,   324,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,     8,    -1,    10,
     376,    -1,    -1,   495,   526,    -1,   271,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,   288,   516,    -1,    -1,   376,    -1,    -1,
      -1,   296,    -1,    -1,   526,    -1,   495,    -1,    -1,    -1,
      -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,   324,
       8,    -1,    10,   495,    -1,    -1,    -1,   526,    -1,    -1,
      -1,    19,    20,    21,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   526,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,   376,    -1,    -1,    -1,    -1,    -1,   239,    -1,   495,
     242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,   239,    -1,   495,   242,    -1,   271,
     526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,   288,   516,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,   271,   526,    -1,    -1,
      -1,    -1,    -1,    -1,   239,    -1,   308,   242,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   296,   324,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,   308,    -1,    -1,   271,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,   324,
     495,   242,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   296,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   308,   376,     8,    -1,    10,    -1,    -1,
     271,   526,    -1,    -1,    -1,    -1,    19,    20,    21,   324,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,   376,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
      -1,   239,    -1,    -1,   242,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,   324,    -1,    -1,    -1,    19,    20,    21,
      -1,   376,    -1,   271,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,    -1,
     308,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   495,    -1,   288,   324,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,   308,    -1,    -1,    -1,    -1,
     495,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,
      -1,   324,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,
      -1,   526,    -1,    -1,     8,    -1,    10,    -1,   239,    -1,
     495,   242,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   376,     8,    -1,    10,    -1,    -1,    -1,
     271,   526,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   495,    -1,   239,   288,    -1,   242,
      -1,    -1,    -1,    -1,    -1,   296,   507,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,   271,    -1,
      -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,   239,    -1,    -1,
     242,    -1,    -1,   296,    -1,    -1,    -1,   495,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,   271,
      -1,   324,    -1,    -1,   522,   376,    -1,    -1,   526,    -1,
      -1,    -1,   495,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,   308,    -1,     8,   522,
      10,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,   324,   376,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,   495,   239,    -1,   271,   242,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,   288,   516,    -1,    -1,    -1,    -1,
      -1,   522,   296,    -1,     8,   526,    10,   271,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    19,    20,    21,    -1,    -1,
      -1,    -1,   495,    -1,   288,    -1,    -1,    -1,    -1,    -1,
     324,    -1,   296,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,   308,    -1,    -1,    -1,    -1,   522,
      -1,    -1,     8,   526,    10,    -1,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
     522,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,    -1,    -1,   242,     8,    -1,    10,    -1,    -1,
      -1,   271,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,   288,    10,
      -1,    -1,    -1,   271,    -1,    -1,   296,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,   495,   239,    -1,   324,   242,    -1,    -1,    -1,    -1,
     308,    -1,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,   324,    -1,   522,    -1,
      -1,   495,   526,    -1,   271,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,   288,   516,    -1,    -1,    -1,   376,    -1,   522,   296,
      -1,     8,   526,    10,    -1,    -1,    -1,   271,    -1,    -1,
      -1,   308,    19,    20,    21,    -1,    -1,    -1,   376,    -1,
      -1,    -1,    -1,   239,   288,    -1,   242,   324,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,   376,
     296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,
      -1,    -1,   376,    -1,    -1,   495,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,   495,   239,    -1,
      -1,   242,   522,    -1,    -1,    -1,   526,    -1,   271,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
     376,    -1,    -1,    -1,   522,   288,    -1,     8,   526,    10,
     271,    -1,    -1,   296,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,   308,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,     8,   495,    10,
      -1,   324,    -1,    -1,    -1,    -1,    -1,   308,    19,    20,
      21,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,   324,    -1,   522,    -1,    -1,    -1,   526,
      -1,   495,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,   376,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   526,    -1,   271,    -1,    -1,   239,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,   495,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,   271,
     516,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     526,    -1,    -1,    -1,    -1,    -1,   288,   324,     8,    -1,
      10,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   324,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,
      -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,   495,    -1,    -1,    -1,   239,   522,
      -1,   242,    -1,   526,   376,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,   522,    -1,    -1,    -1,   526,    -1,    -1,   239,    -1,
     271,   242,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
     271,    -1,     8,    -1,    10,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,   288,    -1,    -1,
      -1,    -1,    -1,   324,    -1,   296,    -1,    -1,   495,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,
      -1,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,   324,    -1,   522,    -1,    -1,    -1,   526,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   376,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,   239,
     522,    -1,   242,    -1,   526,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,    -1,     8,   242,    10,    -1,    -1,
      -1,   271,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,   271,   296,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     296,    -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,   324,    -1,
      -1,    -1,    -1,   239,    -1,    -1,   242,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,   522,    -1,    -1,   495,   526,   376,    -1,    -1,    -1,
      -1,    -1,    -1,   239,    -1,   271,   242,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
     376,   522,   288,    -1,    -1,   526,    -1,    -1,    -1,    -1,
     296,    -1,     8,    -1,    10,   271,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,     8,    -1,    10,    -1,   324,    -1,
     296,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,   324,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,    -1,    -1,   495,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,   495,
     376,    -1,   522,    -1,    -1,    -1,   526,    -1,   271,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,    -1,   288,   522,    -1,    -1,    -1,
     526,    -1,    -1,   296,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,   495,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,   376,    -1,    -1,   522,    -1,    -1,   495,
     526,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,    -1,   239,   522,    -1,   242,    -1,
     526,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   288,    -1,   242,    -1,    -1,   271,    -1,    -1,
     296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,     8,
      -1,    10,   296,   271,    -1,    -1,    -1,    -1,   324,    -1,
      19,    20,    21,    -1,   308,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
     324,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     308,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,   324,    -1,    -1,    -1,
     376,    -1,    -1,   526,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,   239,    -1,
      -1,   242,     8,    -1,    10,    -1,    -1,    -1,   271,    -1,
      -1,    -1,    -1,    19,    20,    21,    -1,    -1,   376,    -1,
      -1,    -1,    -1,    -1,   239,   288,     8,   242,    10,    -1,
     271,    -1,    -1,   296,    -1,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,   308,    -1,   288,    -1,    -1,
      -1,    -1,     8,    -1,    10,   296,   271,    -1,    -1,    -1,
      -1,   324,    -1,    19,    20,    21,    -1,   308,    -1,    -1,
      -1,    -1,    -1,   288,     8,    -1,    10,    -1,    -1,    -1,
      -1,   296,    -1,   324,    -1,    19,    20,    21,    -1,   495,
      -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,   324,
     516,   495,    -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,
     526,    -1,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,   376,    -1,   495,   522,    -1,
     239,    -1,   526,   242,    -1,    -1,    -1,    -1,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,   376,    -1,    -1,   522,    -1,    -1,     8,   526,    10,
      -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,
      -1,    -1,   495,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,   495,    -1,    -1,   239,    -1,    -1,
     242,    -1,    -1,   526,    -1,   271,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,   376,    -1,    -1,
     495,   522,   288,   239,    -1,   526,   242,    -1,    -1,   271,
     296,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,   308,    -1,    -1,   239,   288,    -1,   242,    -1,
      -1,   526,    -1,    -1,   296,   271,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,
      -1,    -1,   288,     8,    -1,    10,    -1,   271,    -1,    -1,
     296,    -1,   324,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,   324,    -1,
     376,    -1,    -1,    -1,   308,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
     324,    -1,    -1,    -1,   376,    -1,   495,    -1,   239,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
     376,    -1,    -1,    -1,     8,    -1,    10,   526,    -1,    -1,
     271,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,     8,   288,    10,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,   324,    -1,    -1,    -1,    -1,    -1,   495,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,   495,    -1,    -1,   522,    -1,    -1,    -1,
     526,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,   376,    -1,    -1,    -1,   495,
     522,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,   495,    -1,    -1,   239,    -1,   522,   242,    -1,    -1,
     526,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,   526,    -1,    -1,    -1,   271,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,   271,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,
      -1,    -1,    -1,    -1,   495,   239,   288,    -1,   242,    -1,
      -1,    -1,    -1,    -1,   296,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,   308,   239,    -1,    -1,
     242,   522,    -1,    -1,    -1,   526,    -1,   271,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    -1,    -1,   288,    -1,    -1,    -1,    -1,   271,
      -1,   239,   296,    -1,   242,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,     8,
     324,    10,    -1,   271,   376,    -1,   308,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,     8,   324,    10,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
     308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,   324,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,   239,    -1,
     495,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,    -1,    -1,    -1,    -1,   522,   376,    -1,
     271,   526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,     8,   308,    10,    -1,
     522,    -1,    -1,    -1,   526,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,     8,    -1,
      10,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,   495,    -1,    -1,    -1,    -1,   522,    -1,
      -1,    -1,   526,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,   376,    -1,    -1,    -1,    -1,
     522,    -1,    -1,    -1,   526,    -1,    -1,   495,    -1,    -1,
     239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,   239,    -1,   522,   242,    -1,     8,   526,    10,
      -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,   271,    -1,    -1,   296,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,    -1,     8,    -1,    10,   324,    -1,    -1,    -1,    -1,
      -1,   308,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   495,    -1,    -1,   324,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   526,    -1,   376,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,   271,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,   288,    10,    -1,    -1,
      -1,   271,    -1,    -1,   296,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,    -1,     8,   308,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,   324,    -1,   495,    -1,   239,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,   495,    -1,
      -1,    -1,    -1,    -1,   376,    -1,    -1,   526,    -1,    -1,
     271,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,    -1,    -1,   522,   376,   288,    -1,   526,
      -1,    -1,    -1,   239,    -1,   296,   242,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,   324,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,     8,    -1,    10,    -1,    -1,    -1,    -1,
     296,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,   324,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,   495,   239,    -1,    -1,   242,
     522,    -1,    -1,    -1,   526,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
     376,    -1,    -1,    -1,    -1,    -1,   526,    -1,   271,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    19,    20,    21,    -1,
     271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   324,    -1,    -1,   495,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,   308,    -1,    -1,
      -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,   239,
      -1,    -1,   242,   324,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,   495,
      -1,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,
      -1,   271,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,   239,    -1,   522,   242,   288,    -1,
     526,    -1,    -1,   271,    -1,   376,   296,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
     288,    -1,    -1,    -1,    -1,    -1,   271,    -1,   296,    -1,
      -1,    -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,
     308,    -1,     8,   288,    10,    -1,    -1,    -1,    -1,    -1,
      -1,   296,    -1,    19,    20,    21,   324,    -1,    -1,    -1,
      -1,    -1,    -1,   308,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,   324,
      -1,    -1,   495,    -1,    -1,    -1,   376,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,   376,   522,
      -1,    -1,    -1,   526,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,   271,    -1,
      -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     8,   288,    10,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,
      -1,   324,    -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,   495,    -1,    -1,
      -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
     495,    -1,    -1,   376,    -1,    -1,    -1,    -1,   526,    -1,
      -1,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   239,    -1,     8,   242,    10,    -1,    -1,
      -1,   526,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,   271,    -1,    -1,
     296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,     8,    -1,    10,    -1,    -1,   271,    -1,    -1,
      -1,    -1,   376,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,     8,    -1,    10,    -1,   271,
      -1,    -1,   296,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,   308,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    -1,    -1,     8,   308,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,   324,    -1,    -1,    -1,   239,    -1,     8,   242,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,
      -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,   271,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,   495,    -1,    -1,   376,   288,    -1,    -1,    -1,    -1,
     526,    -1,    -1,   296,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,   308,    -1,    -1,   522,    -1,
      -1,    -1,   526,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,   324,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,   495,    -1,    -1,   239,    -1,   522,   242,
      -1,    -1,   526,    -1,    -1,   271,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
     522,    -1,   288,    -1,   526,    -1,    -1,    -1,   271,    -1,
     296,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,   308,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,   324,   239,
      -1,    -1,   242,    -1,     8,   308,    10,    -1,   271,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,   324,   495,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,   271,    -1,   296,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,   308,    -1,    -1,   288,   522,
     376,    -1,    -1,   526,    -1,    -1,   296,    -1,    -1,    -1,
      -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
      -1,    -1,    -1,   376,    -1,   239,    -1,     8,   242,    10,
      -1,    -1,    -1,    -1,   324,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,   271,    -1,    -1,
      -1,    -1,    -1,   376,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,     8,    -1,    10,
      -1,    -1,   296,    -1,    -1,    -1,   376,    -1,    19,    20,
      21,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
     324,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,   495,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,   495,    -1,    -1,    -1,   522,    -1,    -1,    -1,
     526,    -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,   376,   516,    -1,    -1,    -1,    -1,    -1,   522,
      -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   495,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,   495,    -1,    -1,    -1,   522,
      -1,    -1,    -1,   526,    -1,    -1,    -1,   271,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   522,    -1,   288,    -1,   526,    -1,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,
     271,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,   288,   239,    -1,
      -1,   242,   526,    -1,    -1,   296,   271,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,    -1,   308,    -1,    -1,
      -1,    -1,   239,   288,     8,   242,    10,    -1,    -1,    -1,
     271,   296,    -1,   324,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,   308,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,   271,   296,    -1,    -1,    -1,   324,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,    -1,     8,   324,    10,   376,    -1,    -1,    -1,    -1,
      -1,   308,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,     8,    -1,
      10,   376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,   495,    10,    -1,    -1,   376,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,     8,    -1,    10,    -1,    -1,   522,   376,
      -1,    -1,   526,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
     495,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,    -1,   495,   239,    -1,   522,   242,    -1,
      -1,   526,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,   495,    -1,
      -1,   522,    -1,    -1,    -1,   526,    -1,   271,    -1,    -1,
      -1,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
      -1,    -1,    -1,    -1,   288,   522,    -1,    -1,    -1,   526,
      -1,    -1,   296,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,
     324,    -1,   242,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,   288,    -1,   242,    -1,    -1,    -1,    -1,    -1,
     296,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,   239,    -1,    -1,   242,   288,    -1,
      -1,    -1,   376,   271,    -1,    -1,   296,    -1,   324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,   308,   242,
     288,    -1,    -1,    -1,    -1,    -1,   271,    -1,   296,    -1,
      -1,    -1,    -1,    -1,   324,    -1,    -1,    -1,   239,    -1,
     308,   242,     8,   288,    10,    -1,    -1,    -1,   271,    -1,
      -1,   296,    -1,    19,    20,    21,   324,    -1,    -1,    -1,
     376,    -1,    -1,   308,     8,   288,    10,    -1,    -1,    -1,
     271,    -1,    -1,   296,    -1,    19,    20,    21,    -1,   324,
      -1,    -1,    -1,    -1,    -1,   308,   376,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,     8,    -1,
      10,   324,    -1,    -1,    -1,    -1,    -1,   308,   376,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,     8,    -1,    10,    -1,    -1,   522,    -1,
      -1,    -1,   526,   376,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   376,    19,    20,    21,   495,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,    -1,   495,   522,    -1,    -1,    -1,
     526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,   495,    -1,    -1,
      -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
     495,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,    -1,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,   495,   239,    -1,    -1,   242,   522,    -1,    -1,
      -1,   526,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,   495,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   526,    -1,   271,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,   522,   288,    -1,    -1,   526,    -1,   271,    -1,   239,
     296,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,   324,    -1,
      -1,   271,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,    -1,    -1,   242,   288,    -1,
     324,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,   239,    -1,   308,   242,
      -1,    -1,    -1,    19,    20,    21,   271,    -1,    -1,    -1,
     376,    -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,   288,    10,    -1,    -1,    -1,   271,    -1,
      -1,   296,   376,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    -1,     8,    -1,    10,   324,
      -1,    -1,    -1,    -1,    -1,   308,   376,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,   324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,   495,
      -1,    -1,    -1,   376,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,   495,    -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,
     526,    -1,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,    -1,
      -1,    -1,   526,    -1,    -1,   495,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,
     495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,   495,   239,    -1,   271,   242,   522,    -1,    -1,
      -1,   526,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,   288,   516,    -1,    -1,    -1,    -1,    -1,    -1,
     296,    -1,    -1,   526,    -1,   271,    -1,   239,    -1,    -1,
     242,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,   324,   239,
     296,    -1,   242,    -1,    -1,    -1,    -1,    -1,    -1,   271,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,    -1,    -1,   242,   288,    -1,   324,    -1,
      -1,   271,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,    -1,   308,   242,   288,    -1,
     376,    -1,    -1,    -1,   271,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,   308,    -1,
       8,   288,    10,    -1,    -1,    -1,   271,    -1,    -1,   296,
     376,    19,    20,    21,   324,    -1,    -1,    -1,    -1,    -1,
      -1,   308,     8,   288,    10,    -1,    -1,    -1,    -1,    -1,
      -1,   296,    -1,    19,    20,    21,    -1,   324,    -1,    -1,
      -1,    -1,    -1,   308,   376,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,   324,
      -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,   376,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,   495,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   376,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,     8,    -1,    10,    -1,    -1,   522,    -1,    -1,   495,
     526,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,
     526,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,   495,    -1,    -1,    -1,    -1,
     522,    -1,    -1,    -1,   526,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,   495,    -1,
      -1,    -1,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
     495,   239,    -1,    -1,   242,   522,    -1,    -1,    -1,   526,
      -1,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,   526,    -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,   242,
     288,    -1,    -1,    -1,    -1,   271,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     308,    -1,   288,    -1,    -1,    -1,    -1,    -1,   271,    -1,
     296,    -1,    -1,    -1,   239,    -1,   324,   242,    -1,    -1,
      -1,    -1,   308,    -1,    -1,   288,    -1,    -1,    -1,    -1,
       8,    -1,    10,   296,    -1,    -1,    -1,    -1,   324,    -1,
      -1,    19,    20,    21,    -1,   308,   271,    -1,    -1,    -1,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,   324,    -1,   288,    -1,     8,    -1,    10,   376,    -1,
      -1,   296,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,   308,   271,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,
      -1,   288,    -1,     8,    -1,    10,    -1,    -1,    -1,   296,
      -1,    -1,    -1,   376,    19,    20,    21,    -1,    -1,    -1,
      -1,   308,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   324,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,   376,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,   495,
      -1,    -1,    -1,    -1,    -1,    -1,   441,    -1,   526,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     526,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,    -1,   526,   242,    -1,    -1,    -1,    -1,    -1,
     495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   271,    -1,    -1,   239,    -1,    -1,   242,
      -1,   526,    -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
     507,   508,   509,   510,   511,   512,   513,   514,   271,   516,
     308,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,   526,
      -1,    -1,    -1,    -1,    -1,   288,   324,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,    -1,    -1,   308,   271,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   324,    -1,   288,    -1,   239,    -1,    -1,   242,    -1,
     271,   296,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,
      -1,    -1,    -1,   308,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,     8,    -1,    10,   296,    -1,   271,    -1,   324,
      -1,    -1,    -1,    19,    20,    21,    -1,   308,    -1,    -1,
      -1,    -1,    -1,   376,   288,     8,    -1,    10,    -1,    -1,
      -1,    -1,   296,   324,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,    -1,
     324,   376,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   376,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,    19,    20,
      21,    -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,
      -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,   526,    -1,    19,    20,    21,    -1,    -1,
     495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,
      -1,   526,    -1,    -1,    -1,    -1,   507,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,   495,    -1,   239,    -1,   526,   242,    -1,    -1,    -1,
      -1,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,   526,    -1,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,   239,    -1,    -1,   242,    -1,   271,    -1,
     296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,   288,   239,    -1,    -1,   242,
      -1,    -1,    -1,   296,    -1,   271,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,   239,    -1,
      -1,   242,   288,     8,    -1,    10,    -1,    -1,   271,    -1,
     296,   324,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   308,    -1,    -1,   288,    -1,    -1,    -1,    -1,
     271,    -1,    -1,   296,    -1,    -1,    -1,    -1,   324,    -1,
     376,    -1,    -1,    -1,    -1,   308,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
      -1,   324,    -1,   376,    -1,   239,    -1,   308,   242,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       8,    -1,    10,   324,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    19,    20,    21,    -1,    -1,    -1,   271,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   376,   288,     8,    -1,    10,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,   308,   376,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
     324,    -1,    -1,    -1,    -1,    -1,    19,    20,    21,   495,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     526,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,   376,   516,    -1,    -1,    -1,    -1,    -1,   495,
      -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,   495,    -1,   239,    -1,    -1,   242,    -1,    -1,
     526,    -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,   495,   518,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   526,    -1,    -1,   271,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,   522,    -1,   288,    -1,   526,    -1,    -1,    -1,    -1,
      -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,   324,
      -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,   271,    -1,    -1,   239,    -1,   522,   242,
      -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,   376,     8,    -1,    10,    -1,   239,    -1,   271,   242,
     308,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,   324,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,     8,    -1,    10,   271,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,     8,    -1,    10,
      -1,   324,    -1,   296,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,   308,    -1,    -1,   376,    -1,
      -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,
      -1,   324,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,   376,    -1,    -1,    19,    20,    21,    -1,
     495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   376,     8,    -1,    10,   522,    -1,    -1,
      -1,   526,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,    -1,
      -1,    -1,   495,   239,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,
      -1,    -1,   495,   526,    -1,   271,   239,    -1,    -1,   242,
      -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,   288,   516,    -1,    -1,    -1,    -1,   239,    -1,
     296,   242,    -1,   526,    -1,    -1,    -1,    -1,   271,    -1,
      -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,   288,    -1,   242,   324,    -1,
     271,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,   239,   288,    -1,   242,
      -1,    -1,    -1,    -1,    -1,   296,   271,    -1,    -1,    -1,
      -1,   324,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,   271,    -1,
     376,   296,    -1,   324,    -1,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,   324,
      -1,    -1,    -1,   376,     8,   308,    10,   271,   239,    -1,
      -1,   242,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,   324,    -1,    -1,   288,   376,     8,    -1,    10,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    19,    20,    21,
     271,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    -1,    -1,    -1,    -1,    -1,   288,    -1,     8,
     324,    10,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
      19,    20,    21,   376,    -1,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,   495,
      -1,    -1,    -1,   324,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,   376,    -1,    -1,     8,   522,    10,    -1,    -1,
     526,    -1,   495,    -1,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,   495,   376,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   526,    -1,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
     495,   522,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,   495,    -1,    -1,    -1,    -1,   522,    -1,    -1,
      -1,   526,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,   526,    -1,   495,    -1,    -1,   271,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,   510,
     511,   512,   513,   514,   288,   516,    -1,    -1,    -1,   271,
     239,    -1,   296,   242,    -1,   526,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,   239,    -1,    -1,   242,
     324,    -1,   271,    -1,    -1,    -1,   308,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,   324,    -1,    -1,    -1,   239,   296,   271,   242,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    19,    20,    21,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,   376,   296,    -1,   324,    -1,     8,   271,    10,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,   376,   288,    -1,    -1,    -1,    -1,
       8,   324,    10,   296,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,   308,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,    -1,   376,    -1,    -1,
      -1,   324,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,   376,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,   495,    -1,    -1,    -1,    -1,   522,    -1,
      -1,    -1,   526,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
     522,    -1,    -1,     8,   526,    10,   495,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
      -1,    -1,   495,   522,    -1,    -1,    -1,   526,    -1,    -1,
      -1,   239,    -1,    -1,   242,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,
      -1,    -1,   495,   526,    -1,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,   271,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,    -1,
     288,   239,    -1,   526,   242,    -1,    -1,    -1,   296,    -1,
     271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     308,    -1,    -1,   239,    -1,    -1,   242,   288,    -1,    -1,
      -1,    -1,    -1,   271,    -1,   296,   324,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,    -1,   308,   242,    -1,
     288,    -1,    -1,    -1,    -1,   271,    -1,    -1,   296,    -1,
      -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,   239,    -1,
     308,   242,   288,    -1,    -1,    -1,    -1,   271,    -1,    -1,
     296,    -1,    -1,    -1,    -1,    -1,   324,    -1,   376,    -1,
      -1,    -1,   308,    -1,   288,    -1,    -1,    -1,    -1,    -1,
     271,    -1,   296,    -1,    -1,    -1,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    -1,   308,   376,    -1,   288,    -1,    -1,
      -1,    -1,     8,    -1,    10,   296,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    19,    20,    21,    -1,   308,   376,    -1,
      -1,    -1,    -1,    -1,   239,    -1,     8,   242,    10,    -1,
      -1,    -1,    -1,   324,    -1,    -1,    -1,    19,    20,    21,
     376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     8,    -1,    10,    -1,   271,    -1,    -1,    -1,
      -1,    -1,   376,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,   430,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   296,    -1,    -1,    -1,   376,    -1,   495,    -1,    -1,
      -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,   324,
       8,    -1,    10,    -1,   495,    -1,    -1,    -1,   526,    -1,
      -1,    19,    20,    21,    -1,    -1,   507,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,   495,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,   495,
      -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     526,    -1,    -1,   507,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,   495,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   526,    -1,    -1,    -1,   507,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,    -1,   526,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,
     242,     8,    -1,    10,    -1,   271,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
     495,    -1,   288,   239,    -1,    -1,   242,    -1,    -1,   271,
     296,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,   308,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,   526,    -1,    -1,   296,   271,    -1,    -1,   324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,     8,    -1,    10,
     296,   239,   324,    -1,   242,    -1,    -1,    -1,    19,    20,
      21,    -1,   308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,   324,    10,
     376,    -1,    -1,   271,    -1,    -1,    -1,    -1,    19,    20,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,   376,    -1,    -1,    -1,   296,    -1,
       8,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     308,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     376,    -1,    -1,     8,    -1,    10,   324,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,    -1,    -1,
      -1,   507,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
     526,    -1,    -1,    -1,   271,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,   518,    -1,    -1,   495,
      -1,   288,    -1,    -1,   526,    -1,    -1,    -1,    -1,   296,
      -1,    -1,   508,   509,   510,   511,   512,   513,   514,    -1,
     516,   308,    -1,    -1,    -1,    -1,   522,    -1,   239,    -1,
     526,   242,    -1,    -1,    -1,    -1,    -1,   324,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   495,   239,    -1,
     271,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     508,   509,   510,   511,   512,   513,   514,   288,   516,    -1,
      -1,    -1,    -1,    -1,   522,   296,    -1,    -1,   526,   376,
     271,   239,    -1,    -1,   242,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    -1,   324,   239,   296,    -1,   242,    -1,    -1,
      -1,    -1,    -1,   271,    -1,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,   324,    -1,     8,   271,    10,   296,   239,
      -1,    -1,   242,    -1,    -1,    -1,    19,    20,    21,    -1,
     308,    -1,    -1,   288,    -1,   376,    -1,    -1,    -1,    -1,
      -1,   296,    -1,    -1,    -1,     8,   324,    10,    -1,    -1,
      -1,   271,    -1,   308,    -1,    -1,    19,    20,    21,    -1,
      -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,   288,   324,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,   495,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
      -1,   508,   509,   510,   511,   512,   513,   514,   376,   516,
      -1,     8,    -1,    10,   324,   522,    -1,    -1,    -1,   526,
      -1,    -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    -1,     8,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    10,    -1,
      -1,    -1,    -1,    -1,   495,    -1,   376,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
      -1,   522,    -1,    -1,   495,   526,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,
     511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,
       8,   522,    10,    -1,    -1,   526,    -1,   495,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
     495,    -1,    -1,    -1,    -1,    -1,   239,    -1,   526,   242,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,    -1,    -1,    -1,    -1,   522,    -1,    -1,
      -1,   526,    -1,    -1,    -1,   495,   239,    -1,   271,   242,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,   509,
     510,   511,   512,   513,   514,   288,   516,    -1,    -1,    -1,
      -1,    -1,   522,   296,    -1,    -1,   526,    -1,   271,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,   324,   239,   296,    -1,   242,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,
      -1,   324,    -1,    -1,   271,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,
     242,   288,    -1,   376,    -1,    -1,   271,    -1,    -1,   296,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   308,    -1,   288,    -1,    -1,    -1,    -1,     8,   271,
      10,   296,    -1,   376,    -1,    -1,    -1,   324,    -1,    19,
      20,    21,    -1,   308,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,   324,
      -1,   239,    -1,     8,   242,    10,   308,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,   376,
       8,    -1,    10,   271,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     288,   376,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     308,    -1,    -1,    -1,   376,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,   324,    -1,    -1,   522,
      -1,    -1,   495,   526,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,     8,    -1,    10,    -1,    -1,   522,
      -1,    -1,    -1,   526,    -1,    19,    20,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,
      -1,    -1,    -1,    -1,     8,    -1,    10,    -1,   495,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    21,    -1,    -1,
      -1,   508,   509,   510,   511,   512,   513,   514,    -1,   516,
     495,    -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,
      -1,    -1,    -1,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,    -1,   495,    -1,    -1,    -1,   522,    -1,   239,
      -1,   526,   242,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,    -1,
      -1,   271,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,   495,    -1,    -1,
      -1,   239,    -1,    -1,   242,    -1,   271,    -1,   308,   507,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,   288,   324,    -1,    -1,    -1,   526,    -1,
      -1,   296,    -1,   271,    -1,     8,    -1,    10,    -1,    -1,
      -1,    -1,    -1,   308,    -1,    -1,    19,    20,    21,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,   324,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     308,    -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   239,   324,    -1,   242,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   376,    -1,    -1,    -1,   239,    -1,   271,   242,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,   288,    -1,    -1,    -1,   376,    -1,
      -1,    -1,   296,    -1,    -1,    -1,     8,   271,    10,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,
     324,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,     8,
      -1,    10,    -1,    -1,   308,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,   495,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,   526,    -1,    -1,    -1,
     495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   507,   508,   509,   510,   511,   512,   513,   514,
      -1,   516,   376,    -1,    -1,    -1,    -1,   495,    -1,    -1,
      -1,   526,    -1,    -1,    -1,    -1,   239,    -1,    -1,   242,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,    -1,   522,    -1,    -1,    -1,   526,     8,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,   271,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,    -1,    -1,
      -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,   324,    -1,   242,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,    -1,
      -1,   495,   526,    -1,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,   271,    -1,   508,   509,   510,   511,   512,   513,
     514,    -1,   516,    -1,    -1,    -1,    -1,    -1,   522,   288,
      -1,    -1,   526,   376,    -1,    -1,    -1,   296,    -1,   271,
     239,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,     8,    -1,
      10,    -1,    -1,    -1,   296,   324,    -1,    -1,    -1,    19,
      20,    21,   271,    -1,    -1,    -1,   308,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,   288,
      10,    -1,   324,    -1,    -1,    -1,    -1,   296,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   324,    -1,    -1,     8,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,
     239,    -1,   495,   242,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,   512,
     513,   514,    -1,   516,    -1,    -1,    -1,   376,     8,    -1,
      10,    -1,   271,   526,    -1,    -1,    -1,    -1,    -1,    19,
      20,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
       8,    -1,    10,    -1,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    19,    20,    21,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   324,   495,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,   526,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   507,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,    -1,   376,    -1,   239,
      -1,    -1,   242,    -1,   526,    -1,   495,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,   239,
      -1,   271,   242,   522,    -1,    -1,    -1,   526,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,
      -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,   308,   239,
      -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,   324,    -1,   296,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,
      -1,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   324,    -1,   495,    -1,   288,   239,
      -1,    -1,   242,    -1,    -1,    -1,   296,    -1,   507,   508,
     509,   510,   511,   512,   513,   514,   376,   516,   308,    -1,
      -1,   239,    -1,    -1,   242,    -1,     8,   526,    10,    -1,
      -1,   271,    -1,    -1,   324,    -1,    -1,    19,    20,    21,
      -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,   288,     8,
      -1,    10,    -1,   271,    -1,    -1,   296,    -1,    -1,    -1,
      19,    20,    21,    -1,    -1,    -1,    -1,    -1,   308,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,   324,    -1,   376,    -1,    -1,    -1,
     308,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   324,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   495,   376,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,   376,    -1,
      -1,    -1,    -1,    -1,    -1,   495,   526,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,   239,    -1,    -1,
     242,    -1,    -1,    -1,    -1,    -1,    -1,   507,   508,   509,
     510,   511,   512,   513,   514,    -1,   516,   495,    -1,    -1,
     239,    -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,   271,
     508,   509,   510,   511,   512,   513,   514,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,   526,    -1,
      -1,    -1,   271,    -1,   296,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   308,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,   308,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   324,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   376,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   376,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   508,   509,   510,   511,
     512,   513,   514,    -1,   516,    -1,   495,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   526,    -1,    -1,    -1,    -1,   508,
     509,   510,   511,   512,   513,   514,    -1,   516,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   526
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     7,    11,    12,    17,    27,    28,    29,    31,    32,
      34,    36,    38,    41,    42,    44,    45,    46,    61,    62,
      63,    67,    68,    73,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,   200,   201,   202,   206,   209,
     210,   211,   220,   229,   230,   231,   233,   234,   235,   241,
     243,   244,   246,   247,   248,   253,   254,   255,   256,   257,
     262,   263,   265,   266,   267,   268,   275,   282,   283,   290,
     291,   310,   311,   313,   315,   316,   318,   319,   320,   321,
     323,   331,   332,   333,   334,   336,   338,   339,   341,   342,
     343,   344,   345,   346,   349,   355,   357,   359,   360,   361,
     365,   366,   372,   374,   375,   379,   380,   381,   382,   383,
     389,   390,   391,   392,   397,   398,   399,   401,   404,   406,
     407,   408,   409,   410,   412,   414,   415,   416,   418,   419,
     422,   423,   429,   433,   434,   436,   440,   450,   459,   460,
     462,   464,   466,   483,   484,   486,   488,   489,   490,   492,
     493,   494,   505,   506,   521,   525,   528,   530,   531,   532,
     538,   539,   540,   571,   572,   573,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   589,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   613,   614,   615,   616,   617,   618,   619,   620,
     621,   622,   623,   624,   625,   626,   627,   628,   629,   630,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,     3,     4,     5,    13,    14,    15,
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
     219,   221,   222,   223,   224,   225,   226,   227,   228,   232,
     236,   237,   238,   240,   245,   249,   250,   251,   252,   258,
     259,   260,   261,   264,   269,   270,   272,   273,   274,   276,
     277,   278,   279,   280,   281,   284,   285,   286,   287,   289,
     292,   293,   294,   295,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   309,   312,   314,   317,   322,   325,
     326,   327,   328,   329,   330,   335,   337,   340,   347,   348,
     350,   351,   352,   353,   354,   356,   362,   363,   364,   367,
     368,   369,   370,   371,   373,   378,   384,   385,   386,   387,
     388,   393,   394,   395,   396,   400,   402,   403,   405,   411,
     413,   417,   420,   421,   424,   425,   426,   427,   428,   431,
     435,   437,   438,   442,   443,   444,   445,   446,   447,   448,
     449,   451,   452,   453,   454,   455,   456,   457,   458,   461,
     463,   465,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   485,   487,
     491,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     511,   512,   519,   521,   538,   539,   540,   544,   548,   563,
     564,   565,   566,   567,   568,   569,   570,   521,   558,   559,
     563,   563,   521,   538,   574,   563,   558,   559,   521,   556,
     563,   521,   552,   563,   521,   552,   552,   563,   552,   552,
     563,   521,   552,   553,   563,   521,   553,   563,   553,   563,
     521,   553,   556,   563,   538,   539,   540,   563,   521,   557,
     563,   552,   552,   521,   553,   556,   557,   563,   538,   538,
     538,   521,   538,   539,   541,   543,   538,   538,   553,   556,
     563,   563,   563,   563,   553,   563,   521,   556,   557,   558,
     559,   563,   521,   558,   563,   556,   557,   558,   559,   553,
     556,   553,   556,   553,   563,   521,   556,   557,   563,   563,
     521,   561,   562,   563,   521,   556,   557,   558,   563,   553,
     563,   521,   538,   540,   554,   555,   563,   538,   540,   554,
     555,   538,   540,   554,   555,   538,   540,   554,   555,   563,
     602,   603,   604,   557,   563,   543,   552,   563,   553,   556,
     553,   563,   553,   563,   538,   552,   538,   553,   563,   553,
     563,   521,   553,   556,   557,   558,   559,   560,   563,   563,
     563,   563,   563,   558,   559,   553,   563,   553,   521,   547,
     552,   563,   552,   552,   552,   552,   556,   521,   557,   558,
     559,   563,   539,   540,   552,   563,   552,   563,   552,   563,
     563,   563,   553,   563,   563,   563,   553,   556,   563,   552,
     563,   556,   553,   563,   521,   552,   557,   563,   556,   557,
     552,   553,   563,   553,   563,   552,   563,   552,   553,   563,
     552,   563,   553,   563,   552,   563,   563,   553,   563,   552,
     563,   553,   563,   563,   563,   553,   556,   557,   558,   559,
     556,   557,   558,   559,   553,   563,   558,   556,   557,   558,
     538,   563,   521,   556,   558,   559,   563,   563,   539,   540,
     563,   588,   539,   563,   552,   552,   563,   563,   552,   552,
     590,   553,   563,   553,   563,   553,   563,   538,   539,   540,
     521,   547,   552,   563,     0,   523,   529,   361,   532,   361,
       5,     6,     9,    75,   307,   377,   431,   432,   517,   533,
     537,   207,   510,     5,     6,     9,    75,   307,   377,   431,
     432,   510,   524,   532,   532,   521,   521,   521,   538,   540,
     521,   521,   521,   521,   563,   552,   552,   521,   552,   552,
     552,   521,   521,   552,   552,   552,   552,   521,   521,   521,
     521,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   521,   521,   521,   521,   552,   521,   521,   521,
     521,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   521,   521,   553,   556,   553,   521,
     552,   521,   552,   552,   552,   521,   521,   521,   521,   552,
     552,   552,   552,   552,   552,   521,   521,   557,   558,   563,
     552,   552,   552,   552,   552,   521,   552,   521,   521,   521,
     521,   552,   552,   552,   521,   521,   521,   521,   521,   521,
     552,   521,   552,   552,   552,   552,   552,   521,   521,   521,
     521,   521,   521,   521,   521,   521,   521,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   521,
     552,   521,   552,   563,   521,   552,   552,   552,   552,   552,
     552,   552,   521,   521,   521,   552,   521,   552,   521,   552,
     521,   552,   521,   552,   552,   521,   521,   521,   521,   521,
     521,   521,   521,   552,   521,   521,   521,   552,   552,   552,
     552,   552,   521,   552,   521,   521,   521,   521,   552,   552,
     521,   552,   521,   521,   521,   521,   521,   521,   521,   521,
     521,   521,   538,   540,   521,   521,   552,   521,   521,   521,
     521,   521,   521,   521,   521,   521,   521,   552,   552,   552,
     552,   552,   552,   552,   521,   521,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   552,   552,
     552,   552,   552,   552,   552,   552,   552,   552,   521,   563,
     499,   500,   519,   545,   546,   547,   549,   563,   538,   539,
     540,   563,     5,   431,   517,   521,   533,   534,   535,   536,
       5,   431,     8,    10,    19,    20,    21,   239,   242,   271,
     288,   296,   308,   324,   376,   495,   508,   509,   510,   511,
     512,   513,   514,   516,   526,   558,   559,   563,   507,   538,
     521,   563,   556,   563,   507,   522,   553,   563,   507,   563,
     507,   207,   510,   553,   563,   207,   557,   563,   507,   563,
     507,   510,   242,   511,   521,   542,   538,   517,   507,   439,
     563,   507,   563,   507,   563,   507,   561,   562,   563,   507,
     563,   507,   554,   555,   563,   507,   521,   560,   563,   507,
     547,   563,   376,   507,   563,   507,   553,   563,   207,   542,
     563,   507,   563,   563,   522,   522,   522,   547,   376,   528,
     361,   563,   563,   563,   563,   563,   563,   518,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   532,    79,
     563,   563,   538,   517,   563,   563,   539,   540,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   507,   563,   563,   563,   563,   556,
     563,   553,   556,   563,   563,   563,   563,   563,   563,   563,
     563,   553,   556,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   556,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   556,   563,   563,   553,   556,   553,   563,   563,
     563,   563,   563,   563,   553,   563,   563,   553,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   563,   553,   547,   520,   507,   520,
     511,   522,   507,   525,   358,   522,   550,   551,   563,   517,
     534,   533,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   563,   522,   522,   563,   522,   550,
     522,   563,   522,   563,   563,   563,   563,   207,   207,   563,
     522,   563,   563,   563,   570,   509,   522,   543,   543,   563,
     563,   563,   522,   522,   563,   563,   522,   522,   538,   540,
     522,   550,   522,   563,   522,   547,   563,   563,   207,   207,
     563,   563,   522,   507,   518,   522,   522,   522,   522,   522,
     522,   522,   522,   522,   522,   507,   522,   522,   507,   507,
     507,   522,   507,   522,   507,   522,   507,   522,   507,   522,
     522,   522,   522,   522,   522,   522,   522,   522,   522,   522,
     507,   507,   563,   507,   522,   522,   522,   507,   522,   522,
     522,   522,   522,   507,   507,   522,   522,   522,   507,   522,
     522,   522,   522,   522,   522,   522,   522,   507,   507,   522,
     522,   522,   507,   522,   507,   522,   522,   522,   522,   522,
     507,   507,   522,   507,   507,   522,   522,   522,   522,   522,
     522,   522,   522,   522,   522,   522,   522,   522,   522,   522,
     522,   507,   522,   522,   522,   522,   522,   522,   522,   522,
     522,   522,   507,   507,   522,   522,   522,   522,   522,   522,
     507,   522,   522,   522,   522,   522,   522,   520,   545,   509,
     525,   507,   518,   521,   522,   507,   525,   507,   522,   507,
     563,   563,   507,   507,   441,   538,   522,   507,   507,   507,
     507,   522,   507,   507,   563,   563,   507,   563,   517,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   507,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   518,   518,
     538,   550,   563,   563,   563,   563,   563,   242,   563,   563,
     563,   563,   563,   563,   563,   518,   563,   522,   507,   522,
     522,   507,   522,   507,   522,   507,   522,   522,   507,   522,
     522,   522,   563,   507,   507,   522,   507,   522,   507,   522,
     522,   507,   507,   522,   522,   522,   507,   507,   507,   522,
     507,   522,   522,   522,   522,   507,   522,   507,   507,   430,
     570,   507,   507,   507,   507,   507,   518,   563,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   549,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   522,   522,   522,   522,   522,   507,   522,   507,
     522,   522,   522,   522,   507,   522,   522,   507,   522,   522,
     507,   507,   507,   507,   563,   563,   563,   563,   563,   563,
     563,   563,   507,   522,   522,   522,   522,   507,   507,   507,
     563,   563,   563,   563,   522,   507,   507,   563,   563,   507,
     563,   507,   563,   507,   563
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
		numberoflists = 0;
		listlenmax=0;
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
		addOp(OP_IN);
	;}
    break;

  case 282:

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

  case 283:

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

  case 284:

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

  case 285:

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

  case 286:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
		addIntOp(OP_PUSHINT,1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
	;}
    break;

  case 287:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
		addIntOp(OP_PUSHINT,-1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
	;}
    break;

  case 288:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_PUSHINT,1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
	;}
    break;

  case 289:

    {
		addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
		addIntOp(OP_PUSHINT,-1);
		addOp(OP_ADD);
		addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
		addIntOp(OP_VAR_GET,varnumber[nvarnumber]);
	;}
    break;

  case 290:

    { addOp(OP_INT); ;}
    break;

  case 291:

    { addOp(OP_FLOAT); ;}
    break;

  case 292:

    { addOp(OP_LENGTH); ;}
    break;

  case 293:

    { addOp(OP_ASC); ;}
    break;

  case 294:

    {
		addIntOp(OP_PUSHINT, 1);	// start
		addIntOp(OP_PUSHINT, 0);	// case sens flag
		addOp(OP_INSTR);
	;}
    break;

  case 295:

    {
		addIntOp(OP_PUSHINT, 0);	// case sens flag
		addOp(OP_INSTR);
	 ;}
    break;

  case 296:

    { addOp(OP_INSTR); ;}
    break;

  case 297:

    {
		addIntOp(OP_PUSHINT, 1);	//start
		addOp(OP_INSTRX);
	;}
    break;

  case 298:

    { addOp(OP_INSTRX); ;}
    break;

  case 299:

    { addOp(OP_CEIL); ;}
    break;

  case 300:

    { addOp(OP_FLOOR); ;}
    break;

  case 301:

    { addOp(OP_SIN); ;}
    break;

  case 302:

    { addOp(OP_COS); ;}
    break;

  case 303:

    { addOp(OP_TAN); ;}
    break;

  case 304:

    { addOp(OP_ASIN); ;}
    break;

  case 305:

    { addOp(OP_ACOS); ;}
    break;

  case 306:

    { addOp(OP_ATAN); ;}
    break;

  case 307:

    { addOp(OP_DEGREES); ;}
    break;

  case 308:

    { addOp(OP_RADIANS); ;}
    break;

  case 309:

    { addOp(OP_LOG); ;}
    break;

  case 310:

    { addOp(OP_LOGTEN); ;}
    break;

  case 311:

    { addOp(OP_SQR); ;}
    break;

  case 312:

    { addOp(OP_EXP); ;}
    break;

  case 313:

    { addOp(OP_ABS); ;}
    break;

  case 314:

    { addOp(OP_RAND); ;}
    break;

  case 315:

    { addFloatOp(OP_PUSHFLOAT, 3.14159265358979323846); ;}
    break;

  case 316:

    {
		addIntOp(OP_PUSHINT, 0);
		addOp(OP_EOF);
	;}
    break;

  case 317:

    { addOp(OP_EOF); ;}
    break;

  case 318:

    { addOp(OP_EXISTS); ;}
    break;

  case 319:

    { addOp(OP_YEAR); ;}
    break;

  case 320:

    { addOp(OP_MONTH); ;}
    break;

  case 321:

    { addOp(OP_DAY); ;}
    break;

  case 322:

    { addOp(OP_HOUR); ;}
    break;

  case 323:

    { addOp(OP_MINUTE); ;}
    break;

  case 324:

    { addOp(OP_SECOND); ;}
    break;

  case 325:

    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 326:

    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 327:

    {
		addIntOp(OP_PUSHINT, 0);
		addOp(OP_SIZE);
	;}
    break;

  case 328:

    { addOp(OP_SIZE); ;}
    break;

  case 329:

    {
		addIntOp(OP_PUSHINT, 0x00);
		addOp(OP_KEYPRESSED);
	;}
    break;

  case 330:

    { addOp(OP_KEYPRESSED); ;}
    break;

  case 331:

    {
		addIntOp(OP_PUSHINT, 0x00);
		addOp(OP_KEY);
	;}
    break;

  case 332:

    {
		addOp(OP_KEY);
	;}
    break;

  case 333:

    { addOp(OP_MOUSEX); ;}
    break;

  case 334:

    { addOp(OP_MOUSEY); ;}
    break;

  case 335:

    { addOp(OP_MOUSEB); ;}
    break;

  case 336:

    { addOp(OP_CLICKX); ;}
    break;

  case 337:

    { addOp(OP_CLICKY); ;}
    break;

  case 338:

    { addOp(OP_CLICKB); ;}
    break;

  case 339:

    { addOp(OP_PIXEL); ;}
    break;

  case 340:

    {
		addIntOp(OP_PUSHINT,255);	// a
		addOp(OP_RGB);
	;}
    break;

  case 341:

    {
		addOp(OP_RGB);
	;}
    break;

  case 342:

    { addOp(OP_GETCOLOR); ;}
    break;

  case 343:

    { addOp(OP_GETBRUSHCOLOR); ;}
    break;

  case 344:

    { addOp(OP_GETPENWIDTH); ;}
    break;

  case 345:

    { addOp(OP_SPRITECOLLIDE); ;}
    break;

  case 346:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_SPRITECOLLIDE); ;}
    break;

  case 347:

    { addOp(OP_SPRITEX); ;}
    break;

  case 348:

    { addOp(OP_SPRITEY); ;}
    break;

  case 349:

    { addOp(OP_SPRITEH); ;}
    break;

  case 350:

    { addOp(OP_SPRITEW); ;}
    break;

  case 351:

    { addOp(OP_SPRITEV); ;}
    break;

  case 352:

    { addOp(OP_SPRITER); ;}
    break;

  case 353:

    { addOp(OP_SPRITES); ;}
    break;

  case 354:

    { addOp(OP_SPRITEO); ;}
    break;

  case 355:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_DBROW);
	;}
    break;

  case 356:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_DBROW);
	;}
    break;

  case 357:

    {
		addOp(OP_DBROW);
	;}
    break;

  case 358:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBINT); ;}
    break;

  case 359:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBINT); ;}
    break;

  case 360:

    {
		addOp(OP_DBINT); ;}
    break;

  case 361:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBFLOAT); ;}
    break;

  case 362:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBFLOAT); ;}
    break;

  case 363:

    {
		addOp(OP_DBFLOAT); ;}
    break;

  case 364:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBNULL); ;}
    break;

  case 365:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBNULL); ;}
    break;

  case 366:

    {
		addOp(OP_DBNULL); ;}
    break;

  case 367:

    { addOp(OP_LASTERROR); ;}
    break;

  case 368:

    { addOp(OP_LASTERRORLINE); ;}
    break;

  case 369:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETDATA); ;}
    break;

  case 370:

    { addOp(OP_NETDATA); ;}
    break;

  case 371:

    { addOp(OP_PORTIN); ;}
    break;

  case 372:

    {
		addIntOp(OP_PUSHINT, 0); // case sens flag
		addOp(OP_COUNT);
	 ;}
    break;

  case 373:

    { addOp(OP_COUNT); ;}
    break;

  case 374:

    { addOp(OP_COUNTX); ;}
    break;

  case 375:

    { addOp(OP_OSTYPE); ;}
    break;

  case 376:

    { addOp(OP_MSEC); ;}
    break;

  case 377:

    { addOp(OP_TEXTWIDTH); ;}
    break;

  case 378:

    { addOp(OP_TEXTBOXWIDTH); ;}
    break;

  case 379:

    { addOp(OP_TEXTHEIGHT); ;}
    break;

  case 380:

    { addOp(OP_TEXTBOXHEIGHT); ;}
    break;

  case 381:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_READBYTE); ;}
    break;

  case 382:

    { addOp(OP_READBYTE); ;}
    break;

  case 383:

    { addOp(OP_FREEDB); ;}
    break;

  case 384:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_FREEDBSET);
	;}
    break;

  case 385:

    { addOp(OP_FREEDBSET); ;}
    break;

  case 386:

    { addOp(OP_FREEFILE); ;}
    break;

  case 387:

    { addOp(OP_FREENET); ;}
    break;

  case 388:

    { addIntOp(OP_PUSHINT, VERSIONSIGNATURE); ;}
    break;

  case 389:

    {
		addIntOp(OP_PUSHINT,-1);	// no default
		addOp(OP_CONFIRM);
	;}
    break;

  case 390:

    {
		addOp(OP_CONFIRM);
	;}
    break;

  case 391:

    {
		addIntOp(OP_PUSHINT,2);	// radix
		addOp(OP_FROMRADIX);
	;}
    break;

  case 392:

    {
		addIntOp(OP_PUSHINT,16);	// radix
		addOp(OP_FROMRADIX);
	;}
    break;

  case 393:

    {
		addIntOp(OP_PUSHINT,8);	// radix
		addOp(OP_FROMRADIX);
	;}
    break;

  case 394:

    {
		addOp(OP_FROMRADIX);
	;}
    break;

  case 395:

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

  case 396:

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

  case 397:

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

  case 398:

    { addOp(OP_WAVLENGTH); ;}
    break;

  case 399:

    { addOp(OP_WAVPOS); ;}
    break;

  case 400:

    { addOp(OP_WAVSTATE); ;}
    break;

  case 401:

    {
		addOp(OP_TYPEOF);
	;}
    break;

  case 402:

    {
		addOp(OP_SOUNDPLAYER);
	;}
    break;

  case 403:

    {
		addIntOp(OP_PUSHINT, 2);	// 2 columns
		addIntOp(OP_PUSHINT, 1);	// 1 row
		addOp(OP_LIST2ARRAY);
		addOp(OP_SOUNDPLAYER);
	;}
    break;

  case 404:

    {
		addOp(OP_SOUNDID);
	;}
    break;

  case 405:

    {
		addOp(OP_SOUNDPOSITION);
	;}
    break;

  case 406:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPOSITION);
	;}
    break;

  case 407:

    {
		addOp(OP_SOUNDSTATE);
	;}
    break;

  case 408:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDSTATE);
	;}
    break;

  case 409:

    {
		addOp(OP_SOUNDLENGTH);
	;}
    break;

  case 410:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDLENGTH);
	;}
    break;

  case 411:

    {
		addOp(OP_SOUNDSAMPLERATE);
	;}
    break;

  case 412:

    {
		addOp(OP_IMAGEWIDTH);
	;}
    break;

  case 413:

    {
		addOp(OP_IMAGEHEIGHT);
	;}
    break;

  case 414:

    {
		addOp(OP_IMAGEPIXEL);
	;}
    break;

  case 415:

    {
		addIntOp(OP_PUSHINT,0);		// default decimal places
		addOp(OP_ROUND);
	;}
    break;

  case 416:

    {
		addOp(OP_ROUND);
	;}
    break;

  case 417:

    {
		addIntOp(OP_VAR_ASSIGNED, varnumber[--nvarnumber]);
	;}
    break;

  case 418:

    {
		addIntOp(OP_ARR_ASSIGNED, varnumber[--nvarnumber]);
	;}
    break;

  case 419:

    {
		addIntOp(OP_ALEN, varnumber[--nvarnumber]);
	;}
    break;

  case 420:

    {
		addIntOp(OP_ALENROWS, varnumber[--nvarnumber]);
	;}
    break;

  case 421:

    {
		addIntOp(OP_ALENCOLS, varnumber[--nvarnumber]);
	;}
    break;

  case 422:

    { addOp(OP_GETARRAYBASE); ;}
    break;

  case 423:

    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 424:

    { addOp(OP_CHR); ;}
    break;

  case 425:

    { addOp(OP_STRING); ;}
    break;

  case 426:

    { addOp(OP_UPPER); ;}
    break;

  case 427:

    { addOp(OP_LOWER); ;}
    break;

  case 428:

    { addOp(OP_MID); ;}
    break;

  case 429:

    { addIntOp(OP_PUSHINT, 1); addOp(OP_MIDX); ;}
    break;

  case 430:

    { addOp(OP_MIDX); ;}
    break;

  case 431:

    { addOp(OP_LEFT); ;}
    break;

  case 432:

    { addOp(OP_RIGHT); ;}
    break;

  case 433:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 434:

    { addOp(OP_READ); ;}
    break;

  case 435:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 436:

    { addOp(OP_READLINE); ;}
    break;

  case 437:

    { addOp(OP_CURRENTDIR); ;}
    break;

  case 438:

    {
		addIntOp(OP_PUSHINT,0);	// default db number
		addOp(OP_STACKSWAP);
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBSTRING); ;}
    break;

  case 439:

    {
		addIntOp(OP_PUSHINT,0);	// default dbset number
		addOp(OP_STACKSWAP);
		addOp(OP_DBSTRING); ;}
    break;

  case 440:

    {
		addOp(OP_DBSTRING); ;}
    break;

  case 441:

    { addOp(OP_LASTERRORMESSAGE); ;}
    break;

  case 442:

    { addOp(OP_LASTERROREXTRA); ;}
    break;

  case 443:

    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETREAD); ;}
    break;

  case 444:

    { addOp(OP_NETREAD); ;}
    break;

  case 445:

    { addOp(OP_NETADDRESS); ;}
    break;

  case 446:

    { addOp(OP_MD5); ;}
    break;

  case 447:

    { addOp(OP_GETCLIPBOARDIMAGE); ;}
    break;

  case 448:

    { addOp(OP_GETCLIPBOARDSTRING); ;}
    break;

  case 449:

    { addOp(OP_GETSETTING); ;}
    break;

  case 450:

    { addOp(OP_DIR); ;}
    break;

  case 451:

    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_DIR); ;}
    break;

  case 452:

    {
		addIntOp(OP_PUSHINT, 0);	// case sens flag
		addOp(OP_REPLACE);
	;}
    break;

  case 453:

    { addOp(OP_REPLACE); ;}
    break;

  case 454:

    { addOp(OP_REPLACEX); ;}
    break;

  case 455:

    {
		addOp(OP_SERIALIZE);
	;}
    break;

  case 456:

    {
		addStringOp(OP_PUSHSTRING, ""); // no delimiter
		addOp(OP_STACKDUP);
		addOp(OP_IMPLODE);
	;}
    break;

  case 457:

    {
		addOp(OP_STACKDUP);				// same delimiter for rows and columns
		addOp(OP_IMPLODE);
	;}
    break;

  case 458:

    {
		addOp(OP_IMPLODE);
	;}
    break;

  case 459:

    {
		addStringOp(OP_PUSHSTRING, "");
		addOp(OP_PROMPT); ;}
    break;

  case 460:

    {
		addOp(OP_PROMPT); ;}
    break;

  case 461:

    {
		addIntOp(OP_PUSHINT,2);	// radix
		addOp(OP_TORADIX);
	;}
    break;

  case 462:

    {
		addIntOp(OP_PUSHINT,16);	// radix
		addOp(OP_TORADIX);
	;}
    break;

  case 463:

    {
		addIntOp(OP_PUSHINT,8);	// radix
		addOp(OP_TORADIX);
	;}
    break;

  case 464:

    {
		addOp(OP_TORADIX);
	;}
    break;

  case 465:

    {
		addOp(OP_DEBUGINFO);
	;}
    break;

  case 466:

    { addOp(OP_ISNUMERIC); ;}
    break;

  case 467:

    { addOp(OP_LTRIM); ;}
    break;

  case 468:

    { addOp(OP_RTRIM); ;}
    break;

  case 469:

    { addOp(OP_TRIM); ;}
    break;

  case 470:

    { addStringOp(OP_PUSHSTRING, IMAGETYPE_BMP); ;}
    break;

  case 471:

    { addStringOp(OP_PUSHSTRING, IMAGETYPE_JPG); ;}
    break;

  case 472:

    { addStringOp(OP_PUSHSTRING, IMAGETYPE_PNG); ;}
    break;

  case 473:

    {
		addOp(OP_SOUNDLOAD);
	;}
    break;

  case 474:

    {
		addIntOp(OP_PUSHINT, 2);	// 2 columns
		addIntOp(OP_PUSHINT, 1);	// 1 row
		addOp(OP_LIST2ARRAY);
		addOp(OP_SOUNDLOAD);
	;}
    break;

  case 475:

    {
		addOp(OP_SOUNDLOADRAW);
	;}
    break;

  case 476:

    {
		addOp(OP_IMAGENEW);
	;}
    break;

  case 477:

    {
		addIntOp(OP_PUSHINT, 0x00);
		addOp(OP_IMAGENEW);
	;}
    break;

  case 478:

    {
		addOp(OP_IMAGELOAD);
	;}
    break;

  case 479:

    {
		addIntOp(OP_PUSHINT, 5); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 480:

    {
		addIntOp(OP_PUSHINT, 4); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 481:

    {
		addIntOp(OP_PUSHINT, 1); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 482:

    {
		addIntOp(OP_PUSHINT, 0); //number of arguments
		addOp(OP_IMAGECOPY);
	;}
    break;

  case 483:

    {
		addStringOp(OP_PUSHSTRING, " ");
		addOp(OP_LJUST);
	;}
    break;

  case 484:

    {
		addOp(OP_LJUST);
	;}
    break;

  case 485:

    {
		addStringOp(OP_PUSHSTRING, " ");
		addOp(OP_RJUST);
	;}
    break;

  case 486:

    {
		addOp(OP_RJUST);
	;}
    break;

  case 487:

    {
		addStringOp(OP_PUSHSTRING, "0");
		addOp(OP_RJUST);
	;}
    break;

  case 488:

    {
		addOp(OP_OPENFILEDIALOG);
	;}
    break;

  case 489:

    {
		addOp(OP_SAVEFILEDIALOG);
	;}
    break;

  case 490:

    {
		addIntOp(OP_VAR_GET, varnumber[--nvarnumber]);
	;}
    break;

  case 491:

    {
		addOp(OP_LIST2ARRAY);
	;}
    break;

  case 492:

    {
		addOp(OP_LIST2MAP);
	;}
    break;

  case 493:

    {
		addOp(OP_UNSERIALIZE);
	;}
    break;

  case 494:

    {
		addIntOp(OP_PUSHINT, 0);	// case sensitive flag
		addOp(OP_EXPLODE);
	;}
    break;

  case 495:

    {
		addOp(OP_EXPLODE);
	;}
    break;

  case 496:

    {
		addOp(OP_EXPLODEX);
	;}
    break;

  case 497:

    {
		addIntOp(OP_PUSHINT, SLICE_ALL);	// get everything
		addOp(OP_GETSLICE);
	;}
    break;

  case 498:

    {
		addOp(OP_GETSLICE);
	;}
    break;

  case 645:

    {
		// start a case block
		newIf(linenumber, IFTABLETYPEBEGINCASE,-1);
	;}
    break;

  case 646:

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

  case 647:

    {
		//
		// add branch to the end if false
		addIntOp(OP_BRANCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
		//
		// put new CASE on the frame for the IF
		newIf(linenumber, IFTABLETYPECASE,-1);
	;}
    break;

  case 648:

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

  case 649:

    {
		//
		// create internal symbol and add to the label table for the top of the loop
		symtableaddress[getInternalSymbol(nextifid,INTERNALSYMBOLTOP)] = wordOffset;
		//
		// add to if frame
		newIf(linenumber, IFTABLETYPEDO, -1);
	;}
    break;

  case 650:

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

  case 651:

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

  case 652:

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

  case 653:

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

  case 654:

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

  case 655:

    {
					//
					// add branch to the end if false
					addIntOp(OP_BRANCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
					//
					// put new if on the frame for the IF
					newIf(linenumber, IFTABLETYPEIF, -1);
			;}
    break;

  case 656:

    {
				// if there is an if branch or jump on the iftable stack get where it is
				// in the wordcode array and then resolve the lable
				if (numifs>0) {
					symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
					numifs--;
				}
			;}
    break;

  case 657:

    {
				//
				// resolve the label on the else to the current location
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset;
				numifs--;
			;}
    break;

  case 658:

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

  case 659:

    {
				//
				// add on error branch
                                addIntOp(OP_ONERRORCATCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// put new if on the frame for the TRY
				newIf(linenumber, IFTABLETYPETRY, -1);
			;}
    break;

  case 660:

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

  case 661:

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

  case 662:

    {
				//
				// create internal symbol and add to the label table for the top of the loop
				newIf(linenumber, IFTABLETYPEWHILE, -1);
				symtableaddress[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE)] = wordOffset;
			;}
    break;

  case 663:

    {
				//
				// add branch to end if false
				addIntOp(OP_BRANCH, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT));
			;}
    break;

  case 670:

    {
				for(int t=numargs-1;t>=0;t--) {
					addIntOp(OP_MAP_DIM, args[t]);
				}
				numargs=0;	// clear the list for next function
			;}
    break;

  case 671:

    {
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
			;}
    break;

  case 672:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 673:

    {
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
			;}
    break;

  case 674:

    {
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 675:

    {
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
			;}
    break;

  case 676:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_DIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 677:

    {
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 678:

    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_ARRAYFILL, varnumber[--nvarnumber]);
			;}
    break;

  case 679:

    {
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
			;}
    break;

  case 680:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 0);		// just fill unassigned
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 681:

    {
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
			;}
    break;

  case 682:

    {
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 1);
				addOp(OP_STACKSWAP);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 0);		// just fill unassigned
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 683:

    {
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
			;}
    break;

  case 684:

    {
				addOp(OP_STACKTOPTO2);
				addIntOp(OP_REDIM, varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT, 0);		// just fill unassigned
				addIntOp(OP_ARRAYFILL, varnumber[nvarnumber]);
			;}
    break;

  case 685:

    {
				addOp(OP_PAUSE);
			;}
    break;

  case 686:

    {
				addOp(OP_THROWERROR);
			;}
    break;

  case 687:

    {
				addOp(OP_CLS);
			;}
    break;

  case 688:

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

  case 689:

    {
				addOp(OP_CLG);
			;}
    break;

  case 690:

    {
				addOp(OP_FASTGRAPHICS);
			;}
    break;

  case 691:

    {
                                addFloatOp(OP_PUSHFLOAT, 1.0);
				addOp(OP_GRAPHSIZE);
			;}
    break;

  case 692:

    {
					addOp(OP_GRAPHSIZE);
			;}
    break;

  case 693:

    {
				addOp(OP_REFRESH);
			;}
    break;

  case 694:

    {
				addOp(OP_END);
			;}
    break;

  case 695:

    {
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 696:

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

  case 697:

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

  case 698:

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

  case 699:

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

  case 700:

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

  case 701:

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

  case 702:

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

  case 703:

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

  case 704:

    {
				addIntOp(OP_PUSHINT, 1);		// fill all elements
				addIntOp(OP_ARRAYFILL, varnumber[--nvarnumber]);
			;}
    break;

  case 705:

    {
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 706:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
			;}
    break;

  case 707:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addIntOp(OP_PUSHINT,-1);
				addOp(OP_ADD);
				addIntOp(OP_VAR_SET,varnumber[nvarnumber]);
			;}
    break;

  case 708:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_ADD);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 709:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_SUB);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 710:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_MUL);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 711:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_DIV);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 712:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_BINARYAND);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 713:

    {
				addIntOp(OP_VAR_GET,varnumber[--nvarnumber]);
				addOp(OP_STACKSWAP);
				addOp(OP_CONCATENATE);
				addIntOp(OP_VAR_SET, varnumber[nvarnumber]);
			;}
    break;

  case 714:

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

  case 715:

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

  case 716:

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

  case 717:

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

  case 718:

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

  case 719:

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

  case 720:

    {
				if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
					errorcode = COMPERR_FUNCTIONGOTO;
					return -1;
				}
				addIntOp(OP_GOTO, varnumber[--nvarnumber]);
			;}
    break;

  case 721:

    {
				if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
					errorcode = COMPERR_FUNCTIONGOTO;
					return -1;
				}
				addIntOp(OP_GOSUB, varnumber[--nvarnumber]);
				addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
			;}
    break;

  case 722:

    {
				addIntOp(OP_PUSHINT, 0); //push number of arguments passed to compare with SUBROUTINE definition
				addIntOp(OP_CALLSUBROUTINE, varnumber[--nvarnumber]);
				addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
			;}
    break;

  case 723:

    {
					addIntOp(OP_PUSHINT, listlen); //push number of arguments passed to compare with SUBROUTINE definition
					addIntOp(OP_CALLSUBROUTINE, varnumber[--nvarnumber]);
					addIntOp(OP_CURRLINE, filenumber * 0x1000000 + linenumber);
			;}
    break;

  case 724:

    {
				addOp(OP_OFFERROR);
			;}
    break;

  case 725:

    {
				addIntOp(OP_ONERRORGOSUB, varnumber[--nvarnumber]);
			;}
    break;

  case 726:

    {
				addIntOp(OP_ONERRORCALL, varnumber[--nvarnumber]);
			;}
    break;

  case 727:

    {
				errorcode = COMPERR_ONERRORCALL;
				return -1;
			;}
    break;

  case 728:

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

  case 729:

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

  case 730:

    {
				addOp(OP_STACKDUP);
				addOp(OP_SETCOLOR);
			;}
    break;

  case 731:

    {
				addOp(OP_SETCOLOR);
			;}
    break;

  case 732:

    {
				addIntOp(OP_PUSHINT, 255);
				addOp(OP_RGB);
				addOp(OP_STACKDUP);
				addOp(OP_SETCOLOR);
				newParseWarning(COMPWARNING_DEPRECATED_FORM);
			;}
    break;

  case 733:

    {
				addIntOp(OP_PUSHINT, 2);	// 2 columns (this)
				addIntOp(OP_PUSHINT, 1);	// 1 row
				addIntOp(OP_PUSHINT, 2);	// 2 columns (max)
				addOp(OP_LIST2ARRAY);
				addOp(OP_SOUND);
			;}
    break;

  case 734:

    {
				addOp(OP_SOUND);
			;}
    break;

  case 735:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPLAY);
	;}
    break;

  case 736:

    {
		addIntOp(OP_PUSHINT, 2);	// 2 columns
		addIntOp(OP_PUSHINT, 1);	// 1 row
		addOp(OP_LIST2ARRAY);
		addOp(OP_SOUNDPLAY);
	;}
    break;

  case 737:

    {
		// either string - lodresource
		// int - player number
		// array - from a variable
		addOp(OP_SOUNDPLAY);
	;}
    break;

  case 738:

    {
		addOp(OP_SOUNDPAUSE);
	;}
    break;

  case 739:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPAUSE);
	;}
    break;

  case 740:

    {
		addOp(OP_SOUNDPLAYEROFF);
	;}
    break;

  case 741:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDPLAYEROFF);
	;}
    break;

  case 742:

    {
		addOp(OP_SOUNDSTOP);
	;}
    break;

  case 743:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDSTOP);
	;}
    break;

  case 744:

    {
		addOp(OP_SOUNDWAIT);
	;}
    break;

  case 745:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_SOUNDWAIT);
	;}
    break;

  case 746:

    {
		addIntOp(OP_PUSHINT,0);
		addOp(OP_SOUNDWAVEFORM);
	;}
    break;

  case 747:

    {
		addOp(OP_SOUNDWAVEFORM);
	;}
    break;

  case 748:

    {
		addOp(OP_SOUNDSYSTEM);
	;}
    break;

  case 749:

    {
		addOp(OP_SOUNDNOENVELOPE);
	;}
    break;

  case 750:

    {
		addOp(OP_SOUNDENVELOPE);
	;}
    break;

  case 751:

    {
		addOp(OP_SOUNDENVELOPE);
	;}
    break;

  case 752:

    {
		addOp(OP_SOUNDNOHARMONICS);
	;}
    break;

  case 753:

    {
		addOp(OP_SOUNDHARMONICS);
	;}
    break;

  case 754:

    {
		addOp(OP_SOUNDHARMONICS);
	;}
    break;

  case 755:

    {
		addOp(OP_SOUNDFADE);
	;}
    break;

  case 756:

    {
		addOp(OP_STACKTOPTO2);
		addOp(OP_STACKTOPTO2);
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_STACKSWAP2);
		addOp(OP_SOUNDFADE);
	;}
    break;

  case 757:

    {
		addOp(OP_SOUNDSEEK);
	;}
    break;

  case 758:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_SOUNDSEEK);
	;}
    break;

  case 759:

    {
		addOp(OP_SOUNDVOLUME);
	;}
    break;

  case 760:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_SOUNDVOLUME);
	;}
    break;

  case 761:

    {
		addOp(OP_SOUNDLOOP);
	;}
    break;

  case 762:

    {
		addIntOp(OP_PUSHINT, -1);
		addOp(OP_STACKSWAP);
		addOp(OP_SOUNDLOOP);
	;}
    break;

  case 763:

    {
				addOp(OP_PLOT);
			;}
    break;

  case 764:

    {
				addOp(OP_LINE);
			;}
    break;

  case 765:

    {
		addOp(OP_CIRCLE);
	;}
    break;

  case 766:

    {
		addOp(OP_ELLIPSE);
	;}
    break;

  case 767:

    {
		addIntOp(OP_PUSHINT, 5); // with bounding circle
		addOp(OP_ARC);
	;}
    break;

  case 768:

    {
		addIntOp(OP_PUSHINT, 6); // with bounding rectangle
		addOp(OP_ARC);
	;}
    break;

  case 769:

    {
		addIntOp(OP_PUSHINT, 5); // with bounding circle
		addOp(OP_CHORD);
	;}
    break;

  case 770:

    {
		addIntOp(OP_PUSHINT, 6); // with bounding rectangle
		addOp(OP_CHORD);
	;}
    break;

  case 771:

    {
		addIntOp(OP_PUSHINT, 5); // with bounding circle
		addOp(OP_PIE);
	;}
    break;

  case 772:

    {
		addIntOp(OP_PUSHINT, 6); // with bounding rectangle
		addOp(OP_PIE);
	;}
    break;

  case 773:

    {
		addOp(OP_RECT);
	;}
    break;

  case 774:

    {
		addOp(OP_STACKDUP);
		addOp(OP_ROUNDEDRECT);
	;}
    break;

  case 775:

    {
		addOp(OP_ROUNDEDRECT);
	;}
    break;

  case 776:

    {
		addOp(OP_TEXT);
	;}
    break;

  case 777:

    {
		addIntOp(OP_PUSHINT, 0); // flags
		addOp(OP_TEXTBOX);
	;}
    break;

  case 778:

    {
		addOp(OP_TEXTBOX);
	;}
    break;

  case 779:

    {
		addOp(OP_FONT);
	;}
    break;

  case 780:

    {
		addIntOp(OP_PUSHINT, 0); // font is not italic
		addOp(OP_FONT);
	;}
    break;

  case 781:

    {
		addIntOp(OP_PUSHINT, -1); // default weight
		addIntOp(OP_PUSHINT, 0); // font is not italic
		addOp(OP_FONT);
	;}
    break;

  case 782:

    {
		addIntOp(OP_PUSHINT, -1); // default size
		addIntOp(OP_PUSHINT, -1); // default weight
		addIntOp(OP_PUSHINT, 0); // font is not italic
		addOp(OP_FONT);
	;}
    break;

  case 783:

    {
				addOp(OP_SAY);
			;}
    break;

  case 784:

    {
				addOp(OP_SYSTEM);
			;}
    break;

  case 785:

    {
				addOp(OP_VOLUME);
			;}
    break;

  case 786:

    {
				addOp(OP_POLY);
			;}
    break;

  case 787:

    {
				addOp(OP_STAMP);
			;}
    break;

  case 788:

    {
				addFloatOp(OP_PUSHFLOAT, 0.0); // default rotate
				addOp(OP_STACKSWAP);
				addOp(OP_STAMP);
			;}
    break;

  case 789:

    {
				addFloatOp(OP_PUSHFLOAT, 1.0); // default scale
				addOp(OP_STACKSWAP);
				addFloatOp(OP_PUSHFLOAT, 0.0); // default rotate
				addOp(OP_STACKSWAP);
				addOp(OP_STAMP);
			;}
    break;

  case 790:

    {
				addIntOp(OP_PUSHINT, 0); // file number zero
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 0); // not binary
				addOp(OP_OPEN);
			;}
    break;

  case 791:

    {
				addIntOp(OP_PUSHINT, 0); // not binary
				addOp(OP_OPEN);
			;}
    break;

  case 792:

    {
				addIntOp(OP_PUSHINT, 0); // file number zero
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 1); // binary
				addOp(OP_OPEN);
			;}
    break;

  case 793:

    {
				addIntOp(OP_PUSHINT, 1); // binary
				addOp(OP_OPEN);
			;}
    break;

  case 794:

    {
				addIntOp(OP_PUSHINT, 9600); // baud
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 795:

    {
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 796:

    {
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 797:

    {
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 798:

    {
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 799:

    {
				addOp(OP_OPENSERIAL);
			;}
    break;

  case 800:

    {
				addIntOp(OP_PUSHINT, 0);  // file number zero
				addOp(OP_STACKSWAP);
				addOp(OP_WRITE);
			;}
    break;

  case 801:

    {
				addOp(OP_WRITE);
			;}
    break;

  case 802:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_WRITELINE);
			;}
    break;

  case 803:

    {
				addOp(OP_WRITELINE);
			;}
    break;

  case 804:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_WRITEBYTE);
			;}
    break;

  case 805:

    {
				addOp(OP_WRITEBYTE);
			;}
    break;

  case 806:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_CLOSE);
			;}
    break;

  case 807:

    {
				addOp(OP_CLOSE);
			;}
    break;

  case 808:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_RESET);
			;}
    break;

  case 809:

    {
				addOp(OP_RESET);
			;}
    break;

  case 810:

    {
				addOp(OP_SEED);
			;}
    break;

  case 811:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_SEEK);
			;}
    break;

  case 812:

    {
				addOp(OP_SEEK);
			;}
    break;

  case 813:

    {
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 814:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 815:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 816:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_UNASSIGNED);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 817:

    {
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 818:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 819:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 820:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_STRING);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 821:

    {
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 822:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 823:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 824:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_INT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 825:

    {
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 826:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_VAR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 827:

    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 828:

    {
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_PUSHINT,T_FLOAT);
				addOp(OP_INPUT);
				addIntOp(OP_ARR_SET, varnumber[--nvarnumber]);
			;}
    break;

  case 829:

    {
				addIntOp(OP_PUSHINT, 0); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 830:

    {
				addIntOp(OP_PUSHINT, 1); //push number of arguments passed
				addIntOp(OP_PUSHINT, 0); // suppress NL
				addOp(OP_PRINT);
			;}
    break;

  case 831:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 832:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 833:

    {
				addIntOp(OP_PUSHINT, 0); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 834:

    {
				addIntOp(OP_PUSHINT, 1); //push number of arguments passed
				addIntOp(OP_PUSHINT, 0); // suppress NL
				addOp(OP_PRINT);
			;}
    break;

  case 835:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 836:

    {
				addIntOp(OP_PUSHINT, listlen); //push number of arguments passed
				addIntOp(OP_PUSHINT, 1); // need NL
				addOp(OP_PRINT);
			;}
    break;

  case 837:

    {
				addOp(OP_WAVPAUSE);
			;}
    break;

  case 838:

    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_WAVPLAY);
			;}
    break;

  case 839:

    {
				addOp(OP_WAVPLAY);
			;}
    break;

  case 840:

    {
				addOp(OP_WAVSEEK);
			;}
    break;

  case 841:

    {
				addOp(OP_WAVSTOP);
			;}
    break;

  case 842:

    {
				addOp(OP_WAVWAIT);
			;}
    break;

  case 843:

    {
				addOp(OP_PUTSLICE);
			;}
    break;

  case 844:

    {
				addIntOp(OP_PUSHINT, 1); // scale
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			;}
    break;

  case 845:

    {
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			;}
    break;

  case 846:

    {
				addOp(OP_IMGLOAD);
			;}
    break;

  case 847:

    {
				addOp(OP_SPRITEDIM);
			;}
    break;

  case 848:

    {
				addOp(OP_SPRITELOAD);
			;}
    break;

  case 849:

    {
				addOp(OP_SPRITESLICE);
			;}
    break;

  case 850:

    {
				addOp(OP_SPRITEPOLY);
			;}
    break;

  case 851:

    {
				addIntOp(OP_PUSHINT,3);	// nr of arguments
				addOp(OP_SPRITEPLACE);
			;}
    break;

  case 852:

    {
				addIntOp(OP_PUSHINT,4);	// nr of arguments
				addOp(OP_SPRITEPLACE);
			;}
    break;

  case 853:

    {
				addIntOp(OP_PUSHINT,5);	// nr of arguments
				addOp(OP_SPRITEPLACE);
			;}
    break;

  case 854:

    {
					addIntOp(OP_PUSHINT,6);	// nr of arguments
					addOp(OP_SPRITEPLACE);
			;}
    break;

  case 855:

    {
				addIntOp(OP_PUSHINT,3);	// nr of arguments
				addOp(OP_SPRITEMOVE);
			;}
    break;

  case 856:

    {
				addIntOp(OP_PUSHINT,4);	// nr of arguments
				addOp(OP_SPRITEMOVE);
			;}
    break;

  case 857:

    {
				addIntOp(OP_PUSHINT,5);	// nr of arguments
				addOp(OP_SPRITEMOVE);
			;}
    break;

  case 858:

    {
					addIntOp(OP_PUSHINT,6);	// nr of arguments
					addOp(OP_SPRITEMOVE);
			;}
    break;

  case 859:

    {
				addOp(OP_SPRITEHIDE);
			;}
    break;

  case 860:

    {
				addOp(OP_SPRITESHOW);
			;}
    break;

  case 861:

    {
				addOp(OP_CLICKCLEAR);
			;}
    break;

  case 862:

    {
				addOp(OP_CHANGEDIR);
			;}
    break;

  case 863:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPEN);
			;}
    break;

  case 864:

    {
				addOp(OP_DBOPEN);
			;}
    break;

  case 865:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_DBCLOSE);
			;}
    break;

  case 866:

    {
				addOp(OP_DBCLOSE);
			;}
    break;

  case 867:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addOp(OP_DBEXECUTE);
			;}
    break;

  case 868:

    {
				addOp(OP_DBEXECUTE);
			;}
    break;

  case 869:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPENSET);
			;}
    break;

  case 870:

    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPENSET);
			;}
    break;

  case 871:

    {
				addOp(OP_DBOPENSET);
			;}
    break;

  case 872:

    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBCLOSESET);
			;}
    break;

  case 873:

    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBCLOSESET);
			;}
    break;

  case 874:

    {
				addOp(OP_DBCLOSESET);
			;}
    break;

  case 875:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_NETLISTEN);
			;}
    break;

  case 876:

    {
				addOp(OP_NETLISTEN);
			;}
    break;

  case 877:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKTOPTO2);
				addOp(OP_NETCONNECT);
			;}
    break;

  case 878:

    {
				addOp(OP_NETCONNECT);
			;}
    break;

  case 879:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_NETWRITE);
			;}
    break;

  case 880:

    {
				addOp(OP_NETWRITE);
			;}
    break;

  case 881:

    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_NETCLOSE);
			;}
    break;

  case 882:

    {
				addOp(OP_NETCLOSE);
			;}
    break;

  case 883:

    {
				addOp(OP_KILL);
			;}
    break;

  case 884:

    {
				addOp(OP_SETCLIPBOARDIMAGE);
			;}
    break;

  case 885:

    {
				addOp(OP_SETCLIPBOARDSTRING);
			;}
    break;

  case 886:

    {
				addOp(OP_SETSETTING);
			;}
    break;

  case 887:

    {
				addOp(OP_PORTOUT);
			;}
    break;

  case 888:

    {
				addStringOp(OP_PUSHSTRING, "PNG");
				addOp(OP_IMGSAVE);
			;}
    break;

  case 889:

    {
				addOp(OP_IMGSAVE);
			;}
    break;

  case 890:

    {
				addOp(OP_EDITVISIBLE);
			;}
    break;

  case 891:

    {
				addOp(OP_GRAPHVISIBLE);
			;}
    break;

  case 892:

    {
				addOp(OP_OUTPUTVISIBLE);
			;}
    break;

  case 893:

    {
				addOp(OP_MAINTOOLBARVISIBLE);
			;}
    break;

  case 894:

    {
				addOp(OP_GRAPHTOOLBARVISIBLE);
			;}
    break;

  case 895:

    {
				addOp(OP_OUTPUTTOOLBARVISIBLE);
			;}
    break;

  case 896:

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

  case 897:

    {
				addOp(OP_PENWIDTH);
			;}
    break;

  case 898:

    {
				addOp(OP_ALERT);
			;}
    break;

  case 899:

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

  case 900:

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

  case 901:

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

  case 902:

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

  case 903:

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

  case 904:

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

  case 905:

    {
				addOp(OP_PRINTERCANCEL);
			;}
    break;

  case 906:

    {
				addOp(OP_PRINTEROFF);
			;}
    break;

  case 907:

    {
				addOp(OP_PRINTERON);
			;}
    break;

  case 908:

    {
				addOp(OP_PRINTERPAGE);
			;}
    break;

  case 909:

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

  case 910:

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

  case 911:

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

  case 912:

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

  case 913:

    {
		addOp(OP_REGEXMINIMAL);
	;}
    break;

  case 914:

    {
		addIntOp(OP_VAR_UN, varnumber[--nvarnumber]);
	;}
    break;

  case 915:

    {
		addIntOp(OP_ARR_UN, varnumber[--nvarnumber]);
	;}
    break;

  case 916:

    {
		addIntOp(OP_VARIABLEWATCH, varnumber[--nvarnumber]);
	;}
    break;

  case 917:

    {
		addOp(OP_IMAGECROP);
	;}
    break;

  case 918:

    {
		addIntOp(OP_PUSHINT,1);	// nr of arguments
		addOp(OP_IMAGEAUTOCROP);
	;}
    break;

  case 919:

    {
		addIntOp(OP_PUSHINT,2);	// nr of arguments
		addOp(OP_IMAGEAUTOCROP);
	;}
    break;

  case 920:

    {
		addIntOp(OP_PUSHINT,3);	// nr of arguments
		addOp(OP_IMAGERESIZE);
	;}
    break;

  case 921:

    {
		addIntOp(OP_PUSHINT,2);	// nr of arguments
		addOp(OP_IMAGERESIZE);
	;}
    break;

  case 922:

    {
		addOp(OP_IMAGESETPIXEL);
	;}
    break;

  case 923:

    {
		addOp(OP_GETCOLOR);
		addOp(OP_IMAGESETPIXEL);
	;}
    break;

  case 924:

    {
		addIntOp(OP_PUSHINT,6);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 925:

    {
		addIntOp(OP_PUSHINT,5);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 926:

    {
		addIntOp(OP_PUSHINT,4);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 927:

    {
		addIntOp(OP_PUSHINT,3);	// nr of arguments
		addOp(OP_IMAGEDRAW);
	;}
    break;

  case 928:

    {
		addIntOp(OP_PUSHINT,6);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 929:

    {
		addIntOp(OP_PUSHINT,5);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 930:

    {
		addIntOp(OP_PUSHINT,4);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 931:

    {
		addIntOp(OP_PUSHINT,3);	// nr of arguments
		addOp(OP_IMAGECENTERED);
	;}
    break;

  case 932:

    {
		addOp(OP_IMAGETRANSFORMED);
	;}
    break;

  case 933:

    {
		addIntOp(OP_PUSHINT,1); // opacity
		addOp(OP_IMAGETRANSFORMED);
	;}
    break;

  case 934:

    {
		addOp(OP_IMAGEROTATE);
	;}
    break;

  case 935:

    {
		addOp(OP_IMAGEFLIP);
	;}
    break;

  case 936:

    {
		addIntOp(OP_PUSHINT,0);
		addOp(OP_IMAGEFLIP);
	;}
    break;

  case 937:

    {
		addOp(OP_IMAGESMOOTH);
	;}
    break;

  case 938:

    {
		addOp(OP_UNLOAD);
	;}
    break;

  case 939:

    {
		addOp(OP_SETGRAPH);
	;}
    break;

  case 940:

    {
		addStringOp(OP_PUSHSTRING, "");
		addOp(OP_SETGRAPH);
	;}
    break;

  case 941:

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

