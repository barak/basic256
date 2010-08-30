
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

/* Line 1676 of yacc.c  */
#line 284 "basicParse.y"

  int number;
  double floatnum;
  char *string;



/* Line 1676 of yacc.c  */
#line 243 "basicParse.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


