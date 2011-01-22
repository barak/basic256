
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

/* Line 1676 of yacc.c  */
#line 265 "basicParse.y"

	int number;
	double floatnum;
	char *string;



/* Line 1676 of yacc.c  */
#line 266 "basicParse.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


