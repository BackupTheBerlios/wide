***************** WIDE ****************
    Wx Inform Development Environment
        http://wide.berlios.de/
          Version 0.99 beta
***************************************

Contents:

0) Intro
1) Setup
2) Projects and project files
3) Tips about using Wide
4) License



========
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



========
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

AUTOCOMPLETION      1 to set autocomplete on, else 0
SHOWLINENUMBERS     1 to have line numbers shown in editor, else 0
WRAPMODE            1 to have automatic word-wrap, else 0
REPLACEHOTKEYS      1 to have automati substitution of hotkeys, else 0

These are settable/resettable inside wide.

---

OBJECT_TREE_SHOW_OBJECTS
OBJECT_TREE_SHOW_PROJECT
OBJECT_TREE_SHOW_GLOBALS
OBJECT_TREE_SHOW_CONSTANTS
OBJECT_TREE_SHOW_FUNCTIONS
OBJECT_TREE_SHOW_CLASSES
OBJECT_TREE_SHOW_INCLUDES
OBJECT_TREE_SHOW_VERBS
OBJECT_TREE_EXPAND_ALL_NODES

For every entry, 1 is to have the correspondent kind of inform items shown in 
the object tree (OBJECT_TREE_SHOW_PROJECT is for project defined classes, the
others are self esplicative).
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

LANGUAGE				Language of the localization

Available values are ENGLISH, ITALIAN, SPANISH, GERMAN, FRENCH

Following the language there are five series - one for each language - of 
definitions of messages, errors and menu entries.

---

[HOTKEYS]
...

Following [HOTHEYS] there's a list of entries used for fast keys substitution.
Entries are in format 
  <ID>=<KEY>,<STRING>
where <ID> is an unique identifier, <KEY> is the key to be pressed and
<STRING> is the string that will replace the pressed key. For example
  H07=à,@`a
means that when the à key is pressed, the @`a string will be inserted in the
edited file.
Also, <KEY> can be the hexadecimal code identifying the key, as in
  H07=E0,@`a

---

[STATEMENTS]
...
[DIRECTIVES]
...
[OTHERKEYWORDS]
...

Following [STATEMENTS], [DIRECTIVES] and [OTHERKEYWORDS] there are three lists 
of keywords used both for autocompletion and for syntax highlighting. Entries
are in the format
  <ID>=<KEYWORD>
where <ID> is an unique identifier.
The three lists are, currently, equivalents (there are future plans to have
them coloured differently).



==============================
2) Projects and projects files
==============================

Wide can handle Inform projects made by different files. Also, in a project
the user can define classes (so that items of that classes are shown in the 
object tree) and additional keywords used for autocompletion and for syntax 
highlighting.

A project is handled with a project file, a text file with .wpf extension and
and a syntax vaguely similar to wide.ini. You can check the example.wpf file.

---

Here's a description of the entries in a wide project file:

---

ZCODEVERSION		Accepted values are 8 and 5. Is the zmachine version used
                    when compiling zcode (z5 or z8, -v5 or -v8 switches) 

---

[FILES]
...

Following [Files] there's a list of file that will be automatically opened
along with the project. All entries are in the format
  <ID>=<FILENAME>
where <ID> is an unique identifier.
The first file of the list is used as "main file". When a project is opened,
a compiling command will always try to compile the main file, instead of the
file shown in the editor. Also, the compiled game and resource file names
depends on main file.

---

[CLASSES]
...

Following [CLASSES] there's a list of classes used in the object tree. As
usual, entries are in the format
  <ID>=<CLASS>
where <ID> is an unique identifier.
As an example, if we have the following entries
  C01=Room
  C02=Prop
then objects of the Room and Prop classes will be shown in the object file
(they won't, otherwise), in separathe branches.

---  

[KEYWORDS]
...

Following [KEYWORDS] there's a list of keywords used both for autocompletion 
and for syntax highlighting. These keywords are added to the [OTHERKEYWORDS]
list defined in the wide.ini file. As usual, entries are in the format
  <ID>=<KEYWORD>
where <ID> is an unique identifier.



========================
3) Tips about using Wide
========================

* You can select block of text by pressing the ALT key while selecting the 
  text with the left mouse.
* You can add up to 5 pdf documents to the About menu. Simply edit the 
  "wide.ini" file and restart WIDE.
* Custom autocompletion list. In the "wide.ini" file you can add keywords to 
  the AUTOCOMPLETION feature. 
  The [AUTOCOMPLETIONLIST] list will enable the syntax highlighting.
* You can zoom-in/out the font dimension with CTRL+MOUSE SCROLL.
* You can shift right-left blocks of text using the TAB and SHIFT+TAB keys.
* It's possible to install WIDE into an USB pen: simply use relative paths for
  the executables.



==========
4) License
==========

Wide is released under the terms of the Gnu Public License 2.0, which can be
found in the attached gpl.txt file, or downloaded from Open Source Initiative 
website: <http://www.opensource.org/>. Everybody can download, use, copy, 
distribute, transfer and modify this software under the terms of the aforesaid 
Gnu Public License. 
The sources of the latest stable version can be downloaded from the Wide
home page: <http://wide.berlios.de/>; the most recent sources can always 
be downloaded from the CVS at the Wide home page.
This software is released ~AS IS~, without warranty of any kind. The authors 
aren't liable for any direct or indirect damage due to the proper or improper 
use of this software.
