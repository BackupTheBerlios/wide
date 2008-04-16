//////////////////////////////////////////////////////////////////////////////
// File:        contrib/samples/stc/prefs.cpp
// Purpose:     STC test Preferences initialization
// Maintainer:  Wyo
// Created:     2003-09-01
// Copyright:   (c) wxGuide
// Licence:     wxWindows licence
// Modified by: silver.slade@tiscali.it
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//! wxWidgets headers

//! wxWidgets/contrib headers

//! application headers
#include "defsext.h"     // Additional definitions
#include "prefs.h"       // Preferences

#define FONTSIZE 9

//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//! language types
const CommonInfo g_CommonPrefs = {
    // editor functionality prefs
    true,  // syntaxEnable
    true,  // foldEnable
    true,  // indentEnable
    // display defaults prefs
    false, // overTypeInitial
    false, // readOnlyInitial
    false,  // wrapModeInitial
    false, // displayEOLEnable
    false, // IndentGuideEnable
    true,  // lineNumberEnable
    true, // longLineOnEnable
    false, // whiteSpaceEnable
};

//----------------------------------------------------------------------------
// keywordlists
// C++
wxChar* InformWordlist1 =
    _T("");
wxChar* InformWordlist2 =
    _T("");
wxChar* InformWordlist3 =
    _T("");




//----------------------------------------------------------------------------
//! languages
const LanguageInfo g_LanguagePrefs [] = {
    // C++
    {_T("INFORM"),
     _T("*.h;*.inf;*.INF;*.H"),
     //wxSTC_LEX_OCTAVE,
     wxSTC_LEX_MATLAB,
     {{mySTC_TYPE_DEFAULT, NULL},
      {mySTC_TYPE_COMMENT, NULL},
      {mySTC_TYPE_COMMENT_LINE, NULL},
      {mySTC_TYPE_COMMENT_DOC, NULL},
      {mySTC_TYPE_NUMBER, NULL},
      {mySTC_TYPE_WORD1, InformWordlist1}, // KEYWORDS
      {mySTC_TYPE_STRING, NULL},
      {mySTC_TYPE_CHARACTER, NULL},
      {mySTC_TYPE_UUID, NULL},
      {mySTC_TYPE_PREPROCESSOR, NULL},
      {mySTC_TYPE_OPERATOR, NULL},
      {mySTC_TYPE_IDENTIFIER, NULL},
      {mySTC_TYPE_STRING_EOL, NULL},
      {mySTC_TYPE_DEFAULT, NULL}, // VERBATIM
      {mySTC_TYPE_REGEX, NULL},
      {mySTC_TYPE_COMMENT_SPECIAL, NULL}, // DOXY
      {mySTC_TYPE_WORD2, InformWordlist2}, // EXTRA WORDS
      {mySTC_TYPE_WORD3, InformWordlist3}, // DOXY KEYWORDS
      {mySTC_TYPE_ERROR, NULL}, // KEYWORDS ERROR
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     mySTC_FOLD_COMMENT | mySTC_FOLD_COMPACT | mySTC_FOLD_PREPROC}    
    };

const int g_LanguagePrefsSize = WXSIZEOF(g_LanguagePrefs);

//----------------------------------------------------------------------------
//! style types
const StyleInfo g_StylePrefs [] = {
    // mySTC_TYPE_DEFAULT
    {_T("Default"),
     _T("BLACK"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

// Stringhe con '
    // mySTC_TYPE_WORD1
    {_T("Keyword1"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_WORD2
    {_T("Keyword2"),
     _T("DARK BLUE"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD3
    {_T("Keyword3"),
     _T("CORNFLOWER BLUE"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD4
    {_T("Keyword4"),
     _T("CYAN"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD5
    {_T("Keyword5"),
     _T("DARK GREY"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_WORD6
    {_T("Keyword6"),
     _T("GREY"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

// COMMENTI
    // mySTC_TYPE_COMMENT
    {_T("Comment"),
     _T("FOREST GREEN"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_COMMENT_DOC
    {_T("Comment (Doc)"),
     _T("BLACK"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_COMMENT_LINE
    {_T("Comment line"),
     _T("FOREST GREEN"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_COMMENT_SPECIAL
    {_T("Special comment"),
     _T("BLACK"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

// testo normale
    // mySTC_TYPE_CHARACTER
    {_T("Character"),
     _T("BLACK"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_CHARACTER_EOL
    {_T("Character (EOL)"),
     _T("KHAKI"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_STRING
    {_T("String"),
     _T("BROWN"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_STRING_EOL
    {_T("String (EOL)"),
     _T("BROWN"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_DELIMITER
    {_T("Delimiter"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_PUNCTUATION
    {_T("Punctuation"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_OPERATOR
    {_T("Operator"),
     _T("BLACK"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_BRACE
    {_T("Label"),
     _T("VIOLET"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_COMMAND
    {_T("Command"),
     _T("BLUE"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_IDENTIFIER
    {_T("Identifier"),
     _T("BLACK"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_LABEL
    {_T("Label"),
     _T("VIOLET"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

// WORDLIST 1
    // mySTC_TYPE_NUMBER
    {_T("Number"),
     _T("NAVY"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_PARAMETER
    {_T("Parameter"),
     _T("VIOLET"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_REGEX
    {_T("Regular expression"),
     _T("ORCHID"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

// Stringhe
    // mySTC_TYPE_UUID
    {_T("UUID"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_VALUE
    {_T("Value"),
     _T("ORCHID"), _T("WHITE"),
     _T(""), FONTSIZE, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_PREPROCESSOR
    {_T("Preprocessor"),
     _T("GREY"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_SCRIPT
    {_T("Script"),
     _T("DARK GREY"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_ERROR
    {_T("Error"),
     _T("RED"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0},

    // mySTC_TYPE_UNDEFINED
    {_T("Undefined"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), FONTSIZE, 0, 0}

    };

const int g_StylePrefsSize = WXSIZEOF(g_StylePrefs);
