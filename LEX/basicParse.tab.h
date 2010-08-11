
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
     PRINT = 258,
     INPUT = 259,
     KEY = 260,
     PIXEL = 261,
     RGB = 262,
     PLOT = 263,
     CIRCLE = 264,
     RECT = 265,
     POLY = 266,
     STAMP = 267,
     LINE = 268,
     FASTGRAPHICS = 269,
     GRAPHSIZE = 270,
     REFRESH = 271,
     CLS = 272,
     CLG = 273,
     IF = 274,
     THEN = 275,
     ELSE = 276,
     ENDIF = 277,
     WHILE = 278,
     ENDWHILE = 279,
     DO = 280,
     UNTIL = 281,
     FOR = 282,
     TO = 283,
     STEP = 284,
     NEXT = 285,
     OPEN = 286,
     READ = 287,
     WRITE = 288,
     CLOSE = 289,
     RESET = 290,
     GOTO = 291,
     GOSUB = 292,
     RETURN = 293,
     REM = 294,
     END = 295,
     SETCOLOR = 296,
     GTE = 297,
     LTE = 298,
     NE = 299,
     DIM = 300,
     REDIM = 301,
     NOP = 302,
     TOINT = 303,
     TOSTRING = 304,
     LENGTH = 305,
     MID = 306,
     LEFT = 307,
     RIGHT = 308,
     UPPER = 309,
     LOWER = 310,
     INSTR = 311,
     CEIL = 312,
     FLOOR = 313,
     RAND = 314,
     SIN = 315,
     COS = 316,
     TAN = 317,
     ASIN = 318,
     ACOS = 319,
     ATAN = 320,
     ABS = 321,
     PI = 322,
     DEGREES = 323,
     RADIANS = 324,
     LOG = 325,
     LOGTEN = 326,
     AND = 327,
     OR = 328,
     XOR = 329,
     NOT = 330,
     PAUSE = 331,
     SOUND = 332,
     ASC = 333,
     CHR = 334,
     TOFLOAT = 335,
     READLINE = 336,
     WRITELINE = 337,
     BOOLEOF = 338,
     MOD = 339,
     INTDIV = 340,
     YEAR = 341,
     MONTH = 342,
     DAY = 343,
     HOUR = 344,
     MINUTE = 345,
     SECOND = 346,
     TEXT = 347,
     FONT = 348,
     SAY = 349,
     SYSTEM = 350,
     VOLUME = 351,
     GRAPHWIDTH = 352,
     GRAPHHEIGHT = 353,
     GETSLICE = 354,
     PUTSLICE = 355,
     IMGLOAD = 356,
     SPRITEDIM = 357,
     SPRITELOAD = 358,
     SPRITESLICE = 359,
     SPRITEMOVE = 360,
     SPRITEHIDE = 361,
     SPRITESHOW = 362,
     SPRITEPLACE = 363,
     SPRITECOLLIDE = 364,
     SPRITEX = 365,
     SPRITEY = 366,
     SPRITEH = 367,
     SPRITEW = 368,
     SPRITEV = 369,
     WAVPLAY = 370,
     WAVSTOP = 371,
     WAVWAIT = 372,
     SIZE = 373,
     SEEK = 374,
     EXISTS = 375,
     BOOLTRUE = 376,
     BOOLFALSE = 377,
     MOUSEX = 378,
     MOUSEY = 379,
     MOUSEB = 380,
     CLICKCLEAR = 381,
     CLICKX = 382,
     CLICKY = 383,
     CLICKB = 384,
     GETCOLOR = 385,
     CLEAR = 386,
     BLACK = 387,
     WHITE = 388,
     RED = 389,
     DARKRED = 390,
     GREEN = 391,
     DARKGREEN = 392,
     BLUE = 393,
     DARKBLUE = 394,
     CYAN = 395,
     DARKCYAN = 396,
     PURPLE = 397,
     DARKPURPLE = 398,
     YELLOW = 399,
     DARKYELLOW = 400,
     ORANGE = 401,
     DARKORANGE = 402,
     GREY = 403,
     DARKGREY = 404,
     CHANGEDIR = 405,
     CURRENTDIR = 406,
     DECIMAL = 407,
     DBOPEN = 408,
     DBCLOSE = 409,
     DBEXECUTE = 410,
     DBOPENSET = 411,
     DBCLOSESET = 412,
     DBROW = 413,
     DBINT = 414,
     DBFLOAT = 415,
     DBSTRING = 416,
     ONERROR = 417,
     OFFERROR = 418,
     LASTERROR = 419,
     LASTERRORMESSAGE = 420,
     LASTERRORLINE = 421,
     LASTERROREXTRA = 422,
     LINENUM = 423,
     INTEGER = 424,
     FLOAT = 425,
     STRING = 426,
     VARIABLE = 427,
     STRINGVAR = 428,
     NEWVAR = 429,
     COLOR = 430,
     LABEL = 431,
     UMINUS = 432
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 283 "basicParse.y"

  int number;
  double floatnum;
  char *string;



/* Line 1676 of yacc.c  */
#line 237 "basicParse.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


