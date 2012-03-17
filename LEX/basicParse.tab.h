
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 266 "basicParse.y"

	int number;
	double floatnum;
	char *string;



/* Line 1676 of yacc.c  */
#line 273 "basicParse.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


