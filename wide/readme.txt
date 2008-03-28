*** WIDE ***
Wx Inform Development Environment
Versione 0.91 beta

Setup:

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

