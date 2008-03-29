***************** WIDE ****************
    Wx Inform Development Environment
        http://wide.berlios.de/
          Version 0.92 beta
***************************************

Contents:

0) Intro
1) Setup
2) Tips about using Wide


0) Intro
========
Wide is a win32 Editor for Inform6: is an Integrated Development Environment 
(IDE), written entirely in C++/WxWidgets, for the creation of text adventures 
based on Graham Nelson's Inform 6 Language. 

With Wide it's possible to edit, compile and run a text adventure in the 
z-code and glulx formats.

Main Features:

* Written in C++ (using the wxWidgets Library)
* Currently, Wide runs on Win32 systems
* Auto-completion window, in edit mode
* Wide is just an exe file: no DLL, no virtual machines. Wide is a "real" 
  stand alone application.
* Wrap lines, show number of lines, code folding
* Inform/Glulx support: wide supports zcode (5 and 8 version), ulx, blb, bres
* Objects tree and Syntax highlight is customizable
* Wide is portable: it can be started from an USB key/pen



1) Setup
========

All the configuration informations are in the wide.ini file. You can edit this
file with a text editor of your choice (for example SciTE or Notepad++).
The wide.ini file should be in the same directory of wide.exe.
Do not edit wide.ini while WIDE is running.
All the paths can be absolute (c:/inform/bin/compiler/inform.exe) or relative
(./bin/compiler/inform.exe).
For all the yes/no values, use 1 for 'yes' and 0 for 'no'

---

Here's a description of the entries in the wide.ini file:

---

WINDOW_LOCATION_X   Do not edit
WINDOW_LOCATION_Y   Do not edit
WINDOW_WIDTH        Do not edit
WINDOW_HEIGHT       Do not edit

These are the position and size of the WIDE window. You shouldn't change them.

---

SHOWTIPS            1 if you want the tips when wide starts, else 0
AUTOCOMPLETION      1 to set autocomplete on, else 0
SHOWLINENUMBERS     1 to have line numbers shown in editor, else 0
WRAPMODE            1 to have automatic word-wrap, else 0

These are settable/resettable inside wide.

---

OBJECT_TREE_SHOW_OBJECTS
OBJECT_TREE_SHOW_GLOBALS
OBJECT_TREE_SHOW_CONSTANTS
OBJECT_TREE_SHOW_FUNCTIONS
OBJECT_TREE_SHOW_CLASSES
OBJECT_TREE_SHOW_INCLUDES
OBJECT_TREE_SHOW_VERBS
OBJECT_TREE_EXPAND_ALL_NODES

For every entry, 1 is to have the correspondent kind of inform items shown in 
the object tree.
The last entry is to have the tree automatically expanded.
These are settable/resettable even inside wide.

---

PDFREADER           Path of a pdf reader installed on your computer
INFORMCOMPILER      Path of inform compiler inform.exe
ZCODEINTERPRETER    Path of the zcode terp used to test zcode games
GLULXINTERPRETER    Path of the glulx terp used to test glulx games
BRESPATH            Path to the iblorb utility bres
BLCPATH             Path to the iblorb utility blc
LIBRARYPATH1        Path to inform library and extensions files
LIBRARYPATH2        Path to inform library and extensions files
LIBRARYPATH3        Path to inform library and extensions files

The pdf reader is used to read manuals and other documents
To compile zcode games, you just need the inform compiler and a zcode 
interpreter (for example WindowsFrotz).
To compile glulxe games, you need the inform compiler and a glulxe
interpreter (for example WindowsGlulxe), but if you want to assemble 
a blorb file, you also need the iblorb utilities bres (to process
resource files) and blc (to assemble blorb file).
You can use up to three different library and extensions directories.

--

PDF1_NAME           Name of the first pdf document
PDF1_PATH           Path to the first pdf document
...

You can consult up to five pdf documents from a wide menu.

PDF2_NAME=The Inform Beginner’s Guide
PDF2_PATH=doc/IBG.pdf

---

AUTOCOMPLETE_NUMBER     Number of characters

Minimum number of character to be written before autocompletion window
appears.

---

[AUTOCOMPLETIONLIST]
...

Following [AUTOCOMPLETIONLIST] there's a list of keyword used both for
autocompletion and for syntax highlighting.


2) Tips about using Wide
========================

* You can select block of text by pressing the ALT key while selecting the text with the left mouse.
* You can add up to 5 pdf documents to the About menu. Simply edit the "wide.ini" file and restart WIDE.
* Custom autocompletion list. In the "wide.ini" file you can add keywords to the AUTOCOMPLETION feature. 
  The [AUTOCOMPLETIONLIST] list will enable the syntax highlighting.
* You can zoom-in/out the font dimension with CTRL+MOUSE SCROLL.
* You can shift right-left blocks of text using the TAB and SHIFT+TAB keys.
* It's possible to install WIDE into an USB pen: simply use relative paths for the executables.
* It's possible to set a Main File for compilation process: create a txt file (in the project directory)
  with the name of main file (only the name without path). This file must have ".wpf" extension. In 
  Wide, open this project file: every time you run compilation, WIDE will compile the main file.
  
  