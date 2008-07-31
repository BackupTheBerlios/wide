//////////////////////////////////////////////////////////////////////////////
// File:        main.cpp
// Purpose:     Wide Inform 6 Editor
// Author:      schillacia@users.berlios.de
// License:     GNU GPL
//////////////////////////////////////////////////////////////////////////////
  #include <wx/wx.h>
  #include "wx/aui/aui.h"
  #include "wx/treectrl.h"
  
  #include "wx/artprov.h"
  #include "wx/menu.h"
  #include "wx/stc/stc.h"  // styled text control
  #include "wx/aboutdlg.h" // Per la Dialog di About
  #include "wx/filedlg.h"  // Per il file chooser
  #include "wx/toolbar.h"  // tool bar principale
  

  #include "wx/regex.h"    // Regular expressions
  #include "defsext.h"     // Additional definitions
  #include "edit.h"        // Edit module
  #include "prefs.h"       // Prefs
  
  #include "wx/fileconf.h" // Gestione file di configurazione

  #include "wx/generic/numdlgg.h" // Gestione input dialog
  #include "main.h" // Gestione file di configurazione


// Icone
  #include "images/wide.xpm"
  #include "images/fileopen.xpm"
  #include "images/filesave.xpm"  
  #include "images/filesaveas.xpm"
  #include "images/filesaveall.xpm" //PL
  #include "images/compilezcode.xpm"    
  #include "images/compileulx.xpm"      
  #include "images/exezcode.xpm"    
  #include "images/exeglulx.xpm"      
  #include "images/undo.xpm"      
  #include "images/redo.xpm"     
  #include "images/findrepl.xpm"   
  #include "images/find.xpm"   
  #include "images/findback.xpm" //PL
  #include "images/findobject.xpm" //PL
  #include "images/findobjectg.xpm" //PL
  #include "images/quit.xpm"     
  #include "images/help.xpm"                        
  #include "images/copy.xpm"                      
  #include "images/cut.xpm"                      
  #include "images/paste.xpm"                       
  #include "images/new.xpm"           
//  #include "images/tipicon.xpm"       
  #include "images/addbookm.xpm"       
  #include "images/forward.xpm"       
  #include "images/back.xpm"             
                      


// COSTANTI
  #define SEP " - "
  #define VERSIONE "0.95 beta"
  #define BUILD " (build 200807221436) "
  #define NOMEAPPLICAZIONE "WIDE"  
  #define DESCRIZIONE "Wx Inform Development Environment"    
  #define CONFIG_FILE "wide.ini"  
    
wxFileConfig *pConfig;       

class MyFrame : public wxFrame {
 enum
    {
        ID_LoadFile = wxID_HIGHEST+1000,
        ID_NewFile,        
        ID_Exit,
        ID_About,
        ID_Comment,
        ID_Uncomment,        
        ID_License,
        ID_Save_File,
        ID_Save_All,        //PL
        ID_NB_Close,
        ID_NextPage,
        ID_PreviousPage,
        ID_ShowObject,
        ID_ShowProject,
        ID_ShowGlobal,
        ID_ShowConstant,
        ID_ShowFunction,
        ID_ShowClass,
        ID_ShowInclude,
        ID_ShowVerb, 
        ID_ExpandAllAlways,
        ID_GotoLine,
        ID_Find,
        ID_FindBack,     //PL
        ID_FindReplace,
        ID_FindObjectLocal,
        ID_FindObjectGlobal,
        ID_NextMarker,
        ID_ToggleMarker,
        ID_ResetMarkers,
        ID_RefreshTree,
        ID_Compile,
        ID_CompileUlx,                
        ID_RunZcode,
        ID_RunUlx,  
        ID_RunBlb,      // AS
        ID_ZcodeVersion5,
        ID_ZcodeVersion8,        
        ID_Doc1,            
        ID_Doc2,            
        ID_Doc3,                            
        ID_Doc4,    
        ID_Doc5,            
        ID_Doc6,
        ID_Doc7,
        ID_Doc8,        
        ID_Tip,              
        ID_OpenProject,           
        ID_SaveProject,         
        ID_CloseProject,
        ID_CreateBlb,    
        ID_MakeAllBlb,
        ID_CreateRes,    
        ID_CreateZBlb,
        ID_RunZBlb,                           
        ID_Fine = ID_Exit+1000
    };    
    
    
 public:
   MyFrame(wxWindow* parent, wxPoint p, int width, int height) : wxFrame(parent, -1, 
        _(NOMEAPPLICAZIONE SEP VERSIONE BUILD),
                      p, wxSize(width,height),
                      wxDEFAULT_FRAME_STYLE)                              
   {
     untitled=0;
     mainFile="";
     LoadConfiguration();
     zcodeversion=".z5";
     zcodeswitch="-v5";
     
     // notify wxAUI which frame to use
     m_mgr.SetManagedWindow(this);
    
     SetIcon(wxIcon(wide_xpm));
    
     // Aggiungo la menubar
     SetMenuBar(CreateMenuBar());
     CreateStatusBar();
     GetStatusBar()->SetStatusText(_("Ready"));
 
/*     console = new wxTextCtrl(this, -1, _(""),
                      wxDefaultPosition, wxSize(100,100),
                      wxNO_BORDER | wxTE_MULTILINE);
     console->SetEditable(false);        */
     
     console = new wxStyledTextCtrl(this, -1,
                     wxDefaultPosition, wxSize(100,100),
                     wxNO_BORDER | wxTE_MULTILINE, "console");
     console->SetReadOnly(true);   
         
     // add the panes to the manager
     tree = CreateTreeCtrl();
     m_mgr.AddPane(tree, wxLEFT, wxT("Object tree"));
     m_mgr.AddPane(auinotebook=CreateNotebook(), wxCENTER);
     m_mgr.AddPane(console, wxBOTTOM, wxT("Ouput window"));
                        
    // TOOL BAR
    toolbar = CreateToolBarCtrl();
    m_mgr.AddPane(toolbar, wxAuiPaneInfo().
          Name(wxT("tb2")).Caption(wxT("Toolbar 2")).
          ToolbarPane().Top().Row(1).
          LeftDockable(false).RightDockable(false));                        
                              
     // tell the manager to "commit" all the changes just made
     m_mgr.Update();

   }
 
   ~MyFrame()
   {
     SaveConfiguration();
     // deinitialize the frame manager
     m_mgr.UnInit();
   }
 
 // Funzioni
 private:
    
    // FILE MENU
    void OnNewFile(wxCommandEvent& evt);    
    void OnLoadFile(wxCommandEvent& evt);
    void LoadFile(wxString path, wxString name);    
    bool checkOpenFile(wxString path);          //AS: check if file is opened
    void setNewStc(Edit* stc);                  //PL: New stc settings
    void OnSaveFile(wxCommandEvent &event); 
    void OnSaveAll(wxCommandEvent &event);       //PL
    void OnExit(wxCommandEvent& evt);
    void OnNextPage(wxCommandEvent& event);
    void OnPreviousPage(wxCommandEvent& event);

    // MENU EDIT
    void OnComment(wxCommandEvent& event);
    void OnUncomment(wxCommandEvent& event);    
                
    // MENU ABOUT
    void OnAbout(wxCommandEvent& evt);
    void OnLicense(wxCommandEvent& evt);
    void OnOptions(wxCommandEvent &event);   
    void LoadConfiguration();   
    void SaveConfiguration();
   
    // Search menu
    void GotoLine(wxCommandEvent &event);   
    void Search(wxCommandEvent &event);
    void SearchBack(wxCommandEvent &event);   //PL
    void SearchObjectLocal(wxCommandEvent &event) { SearchObject(false); };
    void SearchObjectGlobal(wxCommandEvent &event) { SearchObject(true); };
    void SearchObject(bool);   
    size_t SearchRegularExpression(wxString text, wxString pattern);    
    void FindReplace(wxCommandEvent &event);   
    void NextMarker(wxCommandEvent &event);
    void ToggleMarker(wxCommandEvent &event);
    void ResetMarkers(wxCommandEvent &event);
   
    // Tree
    void OnUpdateTree();   
    void OnRefreshTree(wxCommandEvent &event);
    void OnUpdateTreeRegularExpression(wxString text, wxTreeItemId root, wxString nome,  wxString pattern, bool keepquote);
    
    // Metodi lanciati su stc    
    void OnEdit (wxCommandEvent &event);
    void OnSingleEdit (wxCommandEvent &event);    

    // Project Menu
    void OnOpenProject(wxCommandEvent &event);
    void OnSaveProject(wxCommandEvent &event);
    void OnCloseProject(wxCommandEvent &event);
    
    // ZCode menu
    void OnCompile (wxCommandEvent &event);
    void OnRunZcode(wxCommandEvent &event);
    void OnZcodeVersion(wxCommandEvent &event);
    void OnCreateZBlb(wxCommandEvent &event);
    void OnRunZBlb(wxCommandEvent &event);
    // Glulx Menu
    void OnCompileUlx(wxCommandEvent &event);
    void OnRunUlx(wxCommandEvent &event);        
    void OnRunBlb(wxCommandEvent &event);        
    void OnCreateBlb(wxCommandEvent &event);    
    void OnCreateRes(wxCommandEvent &event);        
    void OnMakeAllBlb(wxCommandEvent &event);
        
    // Metodi generici
    void OnClose (wxCloseEvent &event);
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);
    void OnNotebookPageChanged(wxAuiNotebookEvent& evt);
    void OnSelChanged(wxTreeEvent &event);
    
    // Metodi della console
    void OnConsoleClicked(wxStyledTextEvent &event);
    void OnOutput(wxString string);
    void OnClear();

    // HELP MENU
    void OnViewDoc(wxCommandEvent &event);
   
 private:
     wxAuiManager m_mgr;

     wxAuiNotebook* auinotebook;
//     wxTextCtrl* console;
     wxStyledTextCtrl* console;
     wxToolBar* toolbar;
     wxTreeCtrl* tree;
     
     wxMenu* zcodemenu;
     
     wxString lastSearch;
     wxString lastObSearch;

     
     // Impostazioni Object tree
     bool expandAllNodes;
     bool showObjects;    
     bool showProject; 
     bool showGlobals;     
     bool showConstants;     
     bool showFunctions;     
     bool showClasses;     
     bool showIncludes;     
     bool showVerbs;          

     int untitled;
     int autoCompleteNumber;    // Number of char typed before window autocomplete     
     bool autoCompleteSwitch;
     bool showLineNumber;
     bool wrapMode;
     bool hotkeysSwitch;

     // TRANSLATION STUFF
     wxString language;
     wxString MENU_FILE;
     wxString MENU_FILE_NEWFILE; 
     wxString MENU_FILE_OPENFILE;   
     wxString MENU_FILE_SAVEFILE;   
     wxString MENU_FILE_SAVEALL;
     wxString MENU_FILE_NEXTTAB;    
     wxString MENU_FILE_PREVIOUSTAB;
     wxString MENU_FILE_QUIT;    
     
     wxString MENU_EDIT;             
     wxString MENU_EDIT_COPY;            
     wxString MENU_EDIT_CUT;
     wxString MENU_EDIT_PASTE;
     wxString MENU_EDIT_UNDO;
     wxString MENU_EDIT_REDO;
     wxString MENU_EDIT_INDENT;
     wxString MENU_EDIT_UNINDENT;
     wxString MENU_EDIT_COMMENT;
     wxString MENU_EDIT_UNCOMMENT;

     wxString MENU_SEARCH;             
     wxString MENU_SEARCH_FIND;                  
     wxString MENU_SEARCH_FIND_BACKWARDS;                       
     wxString MENU_SEARCH_FIND_REPLACE;                       
     wxString MENU_SEARCH_FIND_OBJECT;
     wxString MENU_SEARCH_FIND_OBJECT_IN_FILE;                                         
     wxString MENU_SEARCH_FIND_GOTO_LINE;                       
     wxString MENU_SEARCH_FIND_NEXT_MARKER;                       
     wxString MENU_SEARCH_FIND_TOGGLE_MARKER;                            
     wxString MENU_SEARCH_FIND_RESET_MARKERS;                            

     
     // INFORM STUFF
     wxString informCompiler;
     wxString libraryPath;     
     wxString zcodeversion;          
     wxString zcodeswitch;    
     
     wxString bres;         
     wxString blc;  
     wxString bext;            
     
     wxString mainFile;      
     wxArrayString projkeywords;
     wxArrayString projclasses;
     
     bool inform_error;            

     // Funzioni di inzializzazione componenti
     wxTreeCtrl* CreateTreeCtrl();      // Definizione creazione albero
     wxMenuBar* CreateMenuBar();
     wxAuiNotebook* CreateNotebook();
     wxToolBar* CreateToolBarCtrl();
     
     bool debugflag;
        
    
     DECLARE_EVENT_TABLE()
 };
 

 
  // our normal wxApp-derived class, as usual
 class MyApp : public wxApp {
 public:
 
   bool OnInit()
   {
     // Load config file
     pConfig = new wxFileConfig(
        NOMEAPPLICAZIONE,
        NOMEAPPLICAZIONE,
        CONFIG_FILE,
        "",
        wxCONFIG_USE_RELATIVE_PATH,
        wxConvUTF8);
     wxConfigBase::Set(pConfig);     
     pConfig->SetPath(_T("/"));
        
     // Window position and dimension
     int width = pConfig->Read(_T("WINDOW_WIDTH"), 1l);
     int height = pConfig->Read(_T("WINDOW_HEIGHT"), 1l);;
     wxPoint point = wxPoint(pConfig->Read(_T("WINDOW_LOCATION_X"), 1l),
                    pConfig->Read(_T("WINDOW_LOCATION_Y"), 1l));
        
     wxFrame* frame = new MyFrame(NULL,point,width,height);
     SetTopWindow(frame);
     frame->Show();
     
     return true;                    
   }
 };
 
 
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // FILE MENU
    EVT_CLOSE (MyFrame::OnClose)
    EVT_MENU(MyFrame::ID_LoadFile,      MyFrame::OnLoadFile)
    EVT_MENU(MyFrame::ID_Exit,          MyFrame::OnExit)
    EVT_MENU(MyFrame::ID_About,         MyFrame::OnAbout)
    EVT_MENU(MyFrame::ID_License,       MyFrame::OnLicense)
    EVT_MENU(MyFrame::ID_NewFile,       MyFrame::OnNewFile)
    EVT_MENU(MyFrame::ID_NextPage,      MyFrame::OnNextPage)
    EVT_MENU(MyFrame::ID_PreviousPage,  MyFrame::OnPreviousPage)
    
    // MENU EDIT
    EVT_MENU (wxID_CUT,                MyFrame::OnSingleEdit)
    EVT_MENU (wxID_COPY,               MyFrame::OnSingleEdit)
    EVT_MENU (wxID_PASTE,              MyFrame::OnSingleEdit)
    
    EVT_MENU (MyFrame::ID_Comment,     MyFrame::OnComment)    
    EVT_MENU (MyFrame::ID_Uncomment,   MyFrame::OnUncomment)        
    
    // Search
    EVT_MENU(MyFrame::ID_GotoLine,     MyFrame::GotoLine)    
    EVT_MENU(MyFrame::ID_Find,         MyFrame::Search)
    EVT_MENU(MyFrame::ID_FindBack,     MyFrame::SearchBack)    //PL
    EVT_MENU(MyFrame::ID_FindObjectLocal,   MyFrame::SearchObjectLocal)    //PL
    EVT_MENU(MyFrame::ID_FindObjectGlobal,  MyFrame::SearchObjectGlobal)    //PL
    EVT_MENU(MyFrame::ID_NextMarker,   MyFrame::NextMarker)
    EVT_MENU(MyFrame::ID_ToggleMarker, MyFrame::ToggleMarker)  
    EVT_MENU(MyFrame::ID_ResetMarkers, MyFrame::ResetMarkers)  
    EVT_MENU(MyFrame::ID_FindReplace,  MyFrame::FindReplace)    //PL    

    
    // Project Menu
    EVT_MENU (ID_OpenProject,            MyFrame::OnOpenProject)
    EVT_MENU (ID_SaveProject,            MyFrame::OnSaveProject)
    EVT_MENU (ID_CloseProject,           MyFrame::OnCloseProject)        
    
    // ZCode Menu    
    EVT_MENU (ID_Compile,            MyFrame::OnCompile)
    EVT_MENU (ID_RunZcode,           MyFrame::OnRunZcode)    
    EVT_MENU (ID_ZcodeVersion5,      MyFrame::OnZcodeVersion)    
    EVT_MENU (ID_ZcodeVersion8,      MyFrame::OnZcodeVersion)
    EVT_MENU (ID_CreateZBlb,         MyFrame::OnCreateZBlb)
    EVT_MENU (ID_RunZBlb,            MyFrame::OnRunZBlb)         
    
    // Glulx Menu
    EVT_MENU (ID_CompileUlx,         MyFrame::OnCompileUlx)
    EVT_MENU (ID_RunUlx,             MyFrame::OnRunUlx)
    EVT_MENU (ID_RunBlb,             MyFrame::OnRunBlb)    
    EVT_MENU (ID_CreateBlb,          MyFrame::OnCreateBlb)    
    EVT_MENU (ID_CreateRes,          MyFrame::OnCreateRes)        
    EVT_MENU (ID_MakeAllBlb,         MyFrame::OnMakeAllBlb)
    
        
    // Object Tree    
    EVT_MENU (ID_RefreshTree,        MyFrame::OnRefreshTree)    
    EVT_MENU (ID_ShowObject,         MyFrame::OnOptions)
    EVT_MENU (ID_ShowProject,        MyFrame::OnOptions)
    EVT_MENU (ID_ShowGlobal,         MyFrame::OnOptions)
    EVT_MENU (ID_ShowConstant,       MyFrame::OnOptions)
    EVT_MENU (ID_ShowFunction,       MyFrame::OnOptions)
    EVT_MENU (ID_ShowClass,          MyFrame::OnOptions)
    EVT_MENU (ID_ShowInclude,        MyFrame::OnOptions)
    EVT_MENU (ID_ShowVerb,           MyFrame::OnOptions)
    EVT_MENU (ID_ExpandAllAlways,    MyFrame::OnOptions)
    
    // Eventi di STC
    EVT_MENU (wxID_FIND,             MyFrame::OnSingleEdit)
    EVT_MENU (myID_INDENTINC,        MyFrame::OnSingleEdit)
    EVT_MENU (myID_INDENTRED,        MyFrame::OnSingleEdit)    
    EVT_MENU (wxID_UNDO,             MyFrame::OnSingleEdit)
    EVT_MENU (wxID_REDO,             MyFrame::OnSingleEdit)
    EVT_MENU (ID_Save_File,          MyFrame::OnSaveFile)
    EVT_MENU (ID_Save_All,           MyFrame::OnSaveAll)      //PL

    // Opzioni
    EVT_MENU (myID_LINENUMBER,       MyFrame::OnEdit)
    EVT_MENU (myID_WRAPMODEON,       MyFrame::OnEdit) 
    EVT_MENU (myID_AUTOCOMPON,       MyFrame::OnEdit) 
    EVT_MENU (myID_HOTKEYSON,        MyFrame::OnEdit)
    
    // Eventi sull'oggetto TREE
    EVT_TREE_SEL_CHANGED(wxID_ANY, MyFrame::OnSelChanged)
    
    // Eventi sulla console
    EVT_STC_DOUBLECLICK(wxID_ANY, MyFrame::OnConsoleClicked)

    // Eventi sull'oggetto notebook
    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MyFrame::OnNotebookPageClose)
    EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, MyFrame::OnNotebookPageChanged)
    
    // DOC MENU
    EVT_MENU (ID_Doc1,           MyFrame::OnViewDoc)        
    EVT_MENU (ID_Doc2,           MyFrame::OnViewDoc)        
    EVT_MENU (ID_Doc3,           MyFrame::OnViewDoc)                
    EVT_MENU (ID_Doc4,           MyFrame::OnViewDoc)                
    EVT_MENU (ID_Doc5,           MyFrame::OnViewDoc) 
    EVT_MENU (ID_Doc6,           MyFrame::OnViewDoc)
    EVT_MENU (ID_Doc7,           MyFrame::OnViewDoc)
    EVT_MENU (ID_Doc8,           MyFrame::OnViewDoc)    

END_EVENT_TABLE()



// Funzioni di classe ---------------------------------------------------------

// MENU EDIT: UNCOMMENT
// NON E' FINALE: cambiare l'algoritmo
void MyFrame::OnUncomment(wxCommandEvent& WXUNUSED(event)){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());

    int start = e->GetSelectionStart();
    int end   = e->GetSelectionEnd();    
    int firstline = e->LineFromPosition(start);
    int lastline = e->LineFromPosition(end);
    for (int i = firstline; i <= lastline; i++)  {
        int linestart = e->PositionFromLine(i);
        int lineend = e->GetLineEndPosition(i);
        wxString tline = e->GetTextRange(linestart, lineend);
        for (size_t j = 0; j < tline.Length(); j++) {
            if (tline[j] == '!') { tline.Remove(j,1); end--; break; }
            if (tline[j] != ' ' && tline[j] != '\t') break;
        }
        //wxString tline = e->GetLine(i);
        e->SetTargetStart(linestart);
        e->SetTargetEnd(lineend);
        e->ReplaceTarget(tline);
        //if (tline.Replace("!","",false)) e->ReplaceTarget(tline);
    }    
    e->SetSelection(start, end);

/*
    // Recupero il testo selezionato e lo modifico, aggiungendo ! all'inizio di ogni riga
    wxString selezionato = e->GetSelectedText();
    if (selezionato.Length() ==0) return;

    // Recupero la selezione corretta anche nel caso in cui non ho selezionato
    int iniziale = e->GetSelectionStart();
    int finale   = e->GetSelectionEnd();

    if (
        (e->GetCharAt(iniziale-1)=='\0' && e->GetCharAt(finale+1)=='\0') ||
        (e->GetCharAt(iniziale-1)=='\r' && e->GetCharAt(finale+1)=='\r') ||
        (e->GetCharAt(iniziale-1)=='\n' && e->GetCharAt(finale+1)=='\n')
        ){
        // REMOVE COMMENT
        selezionato.Replace("!","",false);
        e->ReplaceSelection(selezionato);
    }
    else{
        wxMessageBox (_("Wrong selection for uncomment command"), _("Selection Error"),  wxOK | wxICON_ERROR);
    } */
}

// MENU EDIT: COMMENT
void MyFrame::OnComment(wxCommandEvent& WXUNUSED(event)){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    
    int start = e->GetSelectionStart();
    int end   = e->GetSelectionEnd();
    int firstline = e->LineFromPosition(start);
    int lastline = e->LineFromPosition(end);
    for (int i = firstline; i <= lastline; i++) e->InsertText(e->PositionFromLine(i), "!");
    end = end + lastline - firstline + 1;
    e->SetSelection(start, end);

/*
    // Recupero il testo selezionato e lo modifico, aggiungendo ! all'inizio di ogni riga
    wxString selezionato = e->GetSelectedText();
    if (selezionato.Length() ==0) return;

    // Recupero la selezione corretta anche nel caso in cui non ho selezionato 
    int iniziale = e->GetSelectionStart();
    int finale   = e->GetSelectionEnd();    

    if (
        (e->GetCharAt(iniziale-1)=='\0' && e->GetCharAt(finale+1)=='\0') || 
        (e->GetCharAt(iniziale-1)=='\r' && e->GetCharAt(finale+1)=='\r') ||        
        (e->GetCharAt(iniziale-1)=='\n' && e->GetCharAt(finale+1)=='\n')                 
        ){
        // ADD COMMENT
        wxString commentato = "!";
        commentato = commentato.Append(selezionato);
        // La sostituzione serve perchè non so da dove arriva il file
        // Se editato con altri editor potrei avere solo \n al posto di \r\n
        commentato.Replace("\r\n", "\n", true);
        commentato.Replace("\n", "\r\n", true);
        commentato.Replace("\r\n", "\r\n!", true);
        e->ReplaceSelection(commentato);
    }
    else{
        wxMessageBox (_("Wrong selection for comment command"), _("Selection Error"),  wxOK | wxICON_ERROR);
    }  */  
}



// MENU FILE ****************
void MyFrame::OnNewFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* fd = new wxFileDialog(this, "New File","","","Inform Source Files (*.inf;*.h)|*.inf;*.h|All Files (*.*)|*.*",
    wxFD_SAVE,wxDefaultPosition,wxDefaultSize,"filedlg");
    wxString path,name;
    if (fd->ShowModal() == wxID_OK ){
        path = fd->GetPath();
        name = fd->GetFilename();        
        if (wxFile::Exists(path)){
            wxMessageBox (_("The file already exists."), _("File access Error"),  wxOK | wxICON_ERROR);
            return;
        }
        wxFile file;        
        file.Create(path, false, wxS_DEFAULT);
        Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER, path);
        stc->LoadFile (path);

        setNewStc(stc);

        OnClear();

        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
        auinotebook->AddPage(stc  , name, true , page_bmp);
        //auinotebook->SetSelection(auinotebook->GetSelection()+1);
        OnUpdateTree();
    }
}

// MENU - SEARCH
void MyFrame::Search(wxCommandEvent &event){  //PL
    int position, len, newpos;
    wxString selected;
    // If there's a selection WIDE will search the selected String
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        selected = e->GetSelectedText();
        position = e->GetSelectionEnd(); 
        len = e->GetLength();
        // Else, open a dialog
        if (selected=="") {
            wxTextEntryDialog dialog(this,
                                     _T("Enter a string to find:"),
                                     _T("Search String"),
                                     _T(lastSearch),
                                     wxOK | wxCANCEL);
        
            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!="") {
            lastSearch = selected;
            newpos = e->FindText(position,len,selected,0);
            // If not found, search from beginning 
            if (newpos < 0) newpos = e->FindText(0,position,selected,0);
            if (newpos >= 0) {
                int linea = e->LineFromPosition(newpos);
                e->GotoLine(linea);
                e->SetSelection(newpos,newpos+selected.Length());
            }
        }                
    }
}

void MyFrame::FindReplace(wxCommandEvent &event){   
    wxMessageBox ("Not yet implemented.", _("Message"),  wxOK | wxICON_INFORMATION);           
}

void MyFrame::SearchBack(wxCommandEvent &event){   //PL
    int position, len, newpos;
    wxString selected;
    // If there's a selection WIDE will search the selected String
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        selected = e->GetSelectedText();
        position = e->GetSelectionStart();
        len = e->GetLength();
        // Else, open a dialog
        if (selected=="") {
            wxTextEntryDialog dialog(this,
                                     _T("Enter a string to find:"),
                                     _T("Search String"),
                                     _T(lastSearch),
                                     wxOK | wxCANCEL);

            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!="") {
            lastSearch = selected;
            newpos = e->FindText(position-1,0,selected,0);
            // If not found, search from end
            if (newpos < 0) newpos = e->FindText(len,position,selected,0);
            if (newpos >= 0) {
                int linea = e->LineFromPosition(newpos);
                e->GotoLine(linea);
                e->SetSelection(newpos,newpos+selected.Length());
            }
        }
    }
}

void MyFrame::SearchObject(bool globalflag){  //PL
    int position, len, newpos;
    wxString selected;
    // If there's a selection WIDE will search the selected String
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        selected = e->GetSelectedText();
        position = e->GetSelectionEnd();
        len = e->GetLength();
        // Else, open a dialog
        if (selected=="") {
            wxString caption = "Search Object";
            if (globalflag) caption = "Search Object in Files";
            wxTextEntryDialog dialog(this,
                                     _T("Enter an object to find:"),
                                     _T(caption),
                                     _T(lastObSearch),
                                     wxOK | wxCANCEL);

            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!="") {
            lastObSearch = selected;
            int pannelli = auinotebook->GetPageCount();
            int current = auinotebook->GetSelection();
            int searched = current;
            size_t k = 0;
            Edit* e;
            for (;;) {
                //Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
                e = (Edit*) auinotebook->GetPage(searched);
                wxString text = e->GetText();           
                if (showObjects) k = SearchRegularExpression(text, "\n+[ \t\f\v]*Object[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*" + selected + "[ \t\n\r\f\v]");
                if (showProject) for (size_t i = 0; (k==0) && (i<projclasses.GetCount()); i++) {
                    wxString regexp = "\n+[ \t\f\v]*"+projclasses[i]+"[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*" + selected + "[ \t\n\r\f\v]";
                    //wxString regexp = "\n+[ \t\f\v]*NPC[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*" + selected + "[ \t\n\r\f\v]";                
                    k = SearchRegularExpression(text, regexp);
                }
                if (showGlobals && (k==0)) k = SearchRegularExpression(text, "\n+[ \t\f\v]*Global[ \t\n\r\f\v]+" + selected + "[= \t\n\r\f\v]");
                if (showConstants && (k==0)) k = SearchRegularExpression(text, "\n+[ \t\f\v]*Constant[ \t\n\r\f\v]+" + selected + "[ \t\n\r\f\v]");
                if (showFunctions && (k==0)) k = SearchRegularExpression(text, "[\n\r][ \t\n\r\f\v]*\\[[ \t\n\r\f\v]*" + selected + ".*;");
                if (showClasses && (k==0)) k = SearchRegularExpression(text, "\n+[ \t\f\v]*Class[ \t\n\r\f\v]+" + selected + "[ \t\n\r\f\v]");
                if (showIncludes && (k==0)) k = SearchRegularExpression(text, "\n+[ \t\f\v]*Include[ \t\f\v]+\"" + selected + "(\x2eh)?\"");
                if (showVerbs && (k==0)) k = SearchRegularExpression(text, "\n+[ \t\f\v]*Verb[ \t\f\v]+\'" + selected +"\'");                
                if (k) break;
                if (!globalflag) break;
                searched++; if (searched>=pannelli) searched=0;
                if (searched == current) break;
            }
            auinotebook->SetSelection(current);

            //lastSearch = selected;
            if (k) {
                if (searched != current) auinotebook->SetSelection(searched);
                e = (Edit*) auinotebook->GetPage(searched);
                position = k;
                len = e->GetLength();
                newpos = e->FindText(position,len,selected,0);
                int linea = e->LineFromPosition(newpos);
                e->GotoLine(linea);
                e->SetSelection(newpos,newpos+selected.Length());
            }
        }
    }
}

size_t MyFrame::SearchRegularExpression(wxString text, wxString pattern_global){
    wxRegEx re;
    if (!re.Compile(pattern_global, wxRE_ICASE|wxRE_NEWLINE) ){
        OnOutput("Errore nella RE");
        return 0;
    }
    if ( re.Matches(text) ){
        size_t start, len;
        if (re.GetMatch(&start, &len, 0)){
          if (text[start-1]=='\"') {start--; len++; }
          return start;
        }
    }
    return 0;
}

void MyFrame::NextMarker(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        int line = e->MarkerNext(e->GetCurrentLine()+1,0x01000000);     
        //wxMessageBox (wxString::Format("Sono qui linea %d",line), _("File access Error"),  wxOK | wxICON_ERROR);           
        if (line!=-1){
            // GotoLine
            e->GotoLine(line);
        }
        else{
            // Torno alla prima riga
            line = e->MarkerNext(1,0x01000000);     
            if (line!=-1){
                e->GotoLine(line);                
            }
            else{
                wxMessageBox ("No Markers found.", _("Message"),  wxOK | wxICON_INFORMATION);           
            }
        }
    } 
}

void MyFrame::GotoLine(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        // Richiedo il numero di riga
        long lineNumber = wxGetNumberFromUser( _T(""),_T("Line number:"), _T("Goto Line number"),
                             1, 1, 999999999, this );
        if (lineNumber !=-1){
            e->GotoLine(lineNumber-1);
        }
    }
}

void MyFrame::ToggleMarker(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        //e->MarkerDefine(24, wxSTC_MARK_BOXPLUS,wxNullColour,wxColour(_T("LIGHT GREY")));        
        //wxMessageBox (_("Sono qui"), _("File access Error"),  wxOK | wxICON_ERROR);        
        if (e->MarkerGet(e->GetCurrentLine())==0){
            // Add a new marker
            e->MarkerAdd(e->GetCurrentLine(), 24);
        }
        else{
            // Remove the current marker
            e->MarkerDelete(e->GetCurrentLine(), 24);
        }
    }
}

void MyFrame::ResetMarkers(wxCommandEvent &event){
    if (auinotebook->GetPageCount()==0) return;
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        e->MarkerDeleteAll(-1);
    }
}


void MyFrame::SaveConfiguration() {
     pConfig->Write(_T("OBJECT_TREE_EXPAND_ALL_NODES"), expandAllNodes);
     pConfig->Write(_T("OBJECT_TREE_SHOW_OBJECTS"), showObjects);
     pConfig->Write(_T("OBJECT_TREE_SHOW_PROJECT"), showProject);
     pConfig->Write(_T("OBJECT_TREE_SHOW_GLOBALS"), showGlobals);
     pConfig->Write(_T("OBJECT_TREE_SHOW_CONSTANTS"), showConstants);
     pConfig->Write(_T("OBJECT_TREE_SHOW_FUNCTIONS"), showFunctions);
     pConfig->Write(_T("OBJECT_TREE_SHOW_CLASSES"), showClasses);
     pConfig->Write(_T("OBJECT_TREE_SHOW_INCLUDES"), showIncludes);
     pConfig->Write(_T("OBJECT_TREE_SHOW_VERBS"), showVerbs);

     pConfig->Write(_T("SHOWLINENUMBERS"), showLineNumber);
     pConfig->Write(_T("WRAPMODE"), wrapMode);
     pConfig->Write(_T("AUTOCOMPLETION"), autoCompleteSwitch);
     pConfig->Write(_T("REPLACEHOTKEYS"), hotkeysSwitch);
     
     pConfig->Write(_T("AUTOCOMPLETE_NUMBER"), autoCompleteNumber);
     
     
     // Window location
     int x,y,width,height;
     this->GetPosition(&x,&y);
     this->GetSize(&width,&height);
     pConfig->Write(_T("WINDOW_WIDTH"), width);     
     pConfig->Write(_T("WINDOW_HEIGHT"), height);     
     pConfig->Write(_T("WINDOW_LOCATION_X"), x);          
     pConfig->Write(_T("WINDOW_LOCATION_Y"), y);          
}



void MyFrame::LoadConfiguration() {
     expandAllNodes = pConfig->Read(_T("OBJECT_TREE_EXPAND_ALL_NODES"), 1l) != 0;
     showObjects    = pConfig->Read(_T("OBJECT_TREE_SHOW_OBJECTS"), 1l) != 0;
     showProject    = pConfig->Read(_T("OBJECT_TREE_SHOW_PROJECT"), 1l) != 0;
     showGlobals    = pConfig->Read(_T("OBJECT_TREE_SHOW_GLOBALS"), 1l) != 0;
     showConstants  = pConfig->Read(_T("OBJECT_TREE_SHOW_CONSTANTS"), 1l) != 0;
     showFunctions  = pConfig->Read(_T("OBJECT_TREE_SHOW_FUNCTIONS"), 1l) != 0;
     showClasses    = pConfig->Read(_T("OBJECT_TREE_SHOW_CLASSES"), 1l) != 0;
     showIncludes   = pConfig->Read(_T("OBJECT_TREE_SHOW_INCLUDES"), 1l) != 0;
     showVerbs      = pConfig->Read(_T("OBJECT_TREE_SHOW_VERBS"), 1l) != 0;

     showLineNumber     = pConfig->Read(_T("SHOWLINENUMBERS"), 1l) != 0;
     wrapMode           = pConfig->Read(_T("WRAPMODE"), 1l) != 0; 
     autoCompleteSwitch = pConfig->Read(_T("AUTOCOMPLETION"), 1l) != 0;
     hotkeysSwitch      = pConfig->Read(_T("REPLACEHOTKEYS"), 1l) != 0;
     
     autoCompleteNumber = pConfig->Read(_T("AUTOCOMPLETE_NUMBER"), 1l);
     
     // Inform Stuff
     informCompiler = pConfig->Read("INFORMCOMPILER", _T(""));
     libraryPath="+include_path="+ 
                pConfig->Read("LIBRARYPATH1", _T(""))+_T(",")+
                pConfig->Read("LIBRARYPATH2", _T(""))+_T(",")+
                pConfig->Read("LIBRARYPATH3", _T(""));                
    bres = pConfig->Read("BRESPATH", _T(""));                
    blc  = pConfig->Read("BLCPATH", _T(""));
    bext = pConfig->Read("BLORBEXTENSION", _T(""));
    
    // carico le stringhe in lingua
    language = pConfig->Read("LANGUAGE", _T(""));
    
    // MENU_FILE    
    MENU_FILE             = pConfig->Read(_T(language+"_"+"MENU_FILE"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_FILE"),_T("")):"File";
    MENU_FILE_QUIT        = pConfig->Read(_T(language+"_"+"MENU_FILE_QUIT"),_T(""))        !=""?pConfig->Read(_T(language+"_"+"MENU_FILE_QUIT"),_T("")):"Quit";
    MENU_FILE_NEWFILE     = pConfig->Read(_T(language+"_"+"MENU_FILE_NEWFILE"),_T(""))     !=""?pConfig->Read(_T(language+"_"+"MENU_FILE_NEWFILE"),_T("")):"New File";
    MENU_FILE_OPENFILE    = pConfig->Read(_T(language+"_"+"MENU_FILE_OPENFILE"),_T(""))    !=""?pConfig->Read(_T(language+"_"+"MENU_FILE_OPENFILE"),_T("")):"Open File";
    MENU_FILE_SAVEFILE    = pConfig->Read(_T(language+"_"+"MENU_FILE_SAVEFILE"),_T(""))    !=""?pConfig->Read(_T(language+"_"+"MENU_FILE_SAVEFILE"),_T("")):"Save File";
    MENU_FILE_SAVEALL     = pConfig->Read(_T(language+"_"+"MENU_FILE_SAVEALL"),_T(""))     !=""?pConfig->Read(_T(language+"_"+"MENU_FILE_SAVEALL"),_T("")):"Save All";
    MENU_FILE_NEXTTAB     = pConfig->Read(_T(language+"_"+"MENU_FILE_NEXTTAB"),_T(""))     !=""?pConfig->Read(_T(language+"_"+"MENU_FILE_NEXTTAB"),_T("")):"Next Tab";
    MENU_FILE_PREVIOUSTAB = pConfig->Read(_T(language+"_"+"MENU_FILE_PREVIOUSTAB"),_T("")) !=""?pConfig->Read(_T(language+"_"+"MENU_FILE_PREVIOUSTAB"),_T("")):"Previous Tab";
    
    // MENU EDIT
    MENU_EDIT             = pConfig->Read(_T(language+"_"+"MENU_EDIT"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT"),_T("")):"Edit";
    MENU_EDIT_COPY        = pConfig->Read(_T(language+"_"+"MENU_EDIT_COPY"),_T(""))        !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_COPY"),_T("")):"Copy";
    MENU_EDIT_CUT         = pConfig->Read(_T(language+"_"+"MENU_EDIT_CUT"),_T(""))         !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_CUT"),_T("")):"Cut";
    MENU_EDIT_PASTE       = pConfig->Read(_T(language+"_"+"MENU_EDIT_PASTE"),_T(""))       !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_PASTE"),_T("")):"Paste";
    MENU_EDIT_UNDO        = pConfig->Read(_T(language+"_"+"MENU_EDIT_UNDO"),_T(""))        !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_UNDO"),_T("")):"Undo";
    MENU_EDIT_REDO        = pConfig->Read(_T(language+"_"+"MENU_EDIT_REDO"),_T(""))        !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_REDO"),_T("")):"Redo";
    MENU_EDIT_INDENT      = pConfig->Read(_T(language+"_"+"MENU_EDIT_INDENT"),_T(""))      !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_INDENT"),_T("")):"Indent";
    MENU_EDIT_UNINDENT    = pConfig->Read(_T(language+"_"+"MENU_EDIT_UNINDENT"),_T(""))    !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_UNINDENT"),_T("")):"Unindent";
    MENU_EDIT_COMMENT     = pConfig->Read(_T(language+"_"+"MENU_EDIT_COMMENT"),_T(""))     !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_COMMENT"),_T("")):"Comment";
    MENU_EDIT_UNCOMMENT   = pConfig->Read(_T(language+"_"+"MENU_EDIT_UNCOMMENT"),_T(""))   !=""?pConfig->Read(_T(language+"_"+"MENU_EDIT_UNCOMMENT"),_T("")):"Uncomment";
    
    // SEARCH MENU
    MENU_SEARCH                     = pConfig->Read(_T(language+"_"+"MENU_SEARCH"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH"),_T("")):"Search";
    MENU_SEARCH_FIND                = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND"),_T("")):"Find";
    MENU_SEARCH_FIND_BACKWARDS      = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_BACKWARDS"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_BACKWARDS"),_T("")):"Find Backwards";
    MENU_SEARCH_FIND_REPLACE        = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_REPLACE"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_REPLACE"),_T("")):"Find & Replace";
    MENU_SEARCH_FIND_OBJECT         = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_OBJECT"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_OBJECT"),_T("")):"Find Object";
    MENU_SEARCH_FIND_OBJECT_IN_FILE = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_OBJECT_IN_FILE"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_OBJECT_IN_FILE"),_T("")):"Find Object in File";
    MENU_SEARCH_FIND_GOTO_LINE      = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_GOTO_LINE"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_GOTO_LINE"),_T("")):"Goto Line";
    MENU_SEARCH_FIND_NEXT_MARKER    = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_NEXT_MARKER"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_NEXT_MARKER"),_T("")):"Next Marker";
    MENU_SEARCH_FIND_TOGGLE_MARKER  = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_TOGGLE_MARKER"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_TOGGLE_MARKER"),_T("")):"Toggle Marker";
    MENU_SEARCH_FIND_RESET_MARKERS  = pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_RESET_MARKERS"),_T(""))             !=""?pConfig->Read(_T(language+"_"+"MENU_SEARCH_FIND_RESET_MARKERS"),_T("")):"Reset Markers";
    
    
                                  
}


// Eventi sul singolo EDIT
void MyFrame::OnSingleEdit (wxCommandEvent &evt) {
    // Recupero l'edit attuale e processo l'evento
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    if (e) {
        e->ProcessEvent (evt);
    }        
}


// Save file
void MyFrame::OnSaveFile (wxCommandEvent &evt) {
    if (auinotebook->GetPageCount()==0){
        return;
    }
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());    
    if (!e->Modified()) return;
    /*PL    wxMessageBox (_("There is nothing to save!"), _("Save file"),
                      wxOK | wxICON_EXCLAMATION);
        return;
    } */
    wxString nomeprima = e->GetFilename();
    e->SaveFile();
    
    // Aggiorno il nome del tab (in caso di rinominazione del file
    wxString nome = e->GetFilename();
//    wxString sep = "/";
    if (nome!=nomeprima){
        nome = nome.Mid(nome.Find('\\',true)+1,nome.Len());
        auinotebook->SetPageText(auinotebook->GetSelection(), nome);
    }
    
    // Aggiorno la status bar
    GetStatusBar()->SetStatusText(_("File saved."));
    
    // Aggiorno anche l'albero degli oggetti
    OnUpdateTree();
}

void MyFrame::OnSaveAll (wxCommandEvent &evt) {     //PL
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli-1 ;i++) { 
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString titolo = auinotebook->GetPageText(i);
        if (e) {
            if (e->Modified()) e->SaveFile();
            if (e->Modified()) {
                wxMessageBox (_("Text could not be saved!"), _("Close abort"),
                 wxOK | wxICON_EXCLAMATION);
                continue;
            }
        }
    }
    // Aggiorno la status bar
    GetStatusBar()->SetStatusText(_("Files saved."));

    // Aggiorno anche l'albero degli oggetti
    OnUpdateTree();
}


void MyFrame::OnNextPage(wxCommandEvent &evt) {
    int pannelli = auinotebook->GetPageCount();
    int current = auinotebook->GetSelection();
    current++; if (current>=pannelli) current=0;
    auinotebook->SetSelection(current);
}


void MyFrame::OnPreviousPage(wxCommandEvent &evt) {
    int pannelli = auinotebook->GetPageCount();
    int current = auinotebook->GetSelection();
    current--; if (current<0) current=pannelli-1;
    auinotebook->SetSelection(current);
}




// Impostazioni Object Tree
void MyFrame::OnRefreshTree(wxCommandEvent &event){
    OnUpdateTree(); 
}


void MyFrame::OnOptions(wxCommandEvent &event){
    int id = event.GetId();
//    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION);
    switch (id)
    {
        case ID_ShowObject: showObjects=event.IsChecked(); OnUpdateTree(); break;
        case ID_ShowProject: showProject=event.IsChecked(); OnUpdateTree(); break;
        case ID_ShowGlobal: showGlobals=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowConstant: showConstants=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowFunction: showFunctions=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowClass: showClasses=event.IsChecked(); OnUpdateTree(); break;                        
        case ID_ShowInclude: showIncludes=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowVerb: showVerbs=event.IsChecked(); OnUpdateTree(); break;                 
        case ID_ExpandAllAlways: expandAllNodes=event.IsChecked(); OnUpdateTree(); break;                                

    }
}

// MENU DOC
void MyFrame::OnViewDoc(wxCommandEvent &event){
    int id = event.GetId();
//    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION);
    wxString documento;
    wxString comando;
    switch (id)
    {
        case ID_Doc1: documento = pConfig->Read("PDF1_PATH", _T("")); break;
        case ID_Doc2: documento = pConfig->Read("PDF2_PATH", _T("")); break;
        case ID_Doc3: documento = pConfig->Read("PDF3_PATH", _T("")); break;
        case ID_Doc4: documento = pConfig->Read("PDF4_PATH", _T("")); break;
        case ID_Doc5: documento = pConfig->Read("PDF5_PATH", _T("")); break;
        case ID_Doc6: documento = pConfig->Read("PDF6_PATH", _T("")); break;
        case ID_Doc7: documento = pConfig->Read("PDF7_PATH", _T("")); break;
        case ID_Doc8: documento = pConfig->Read("PDF8_PATH", _T("")); break;
    }
    
    // Visualizzo il pdf o word   
    //wxString comando = pConfig->Read("PDFREADER", _T(""))+" \""+documento+"\"";
    wxString pdfreader = pConfig->Read("PDFREADER", _T(""));
    if (pdfreader == "") {
        comando = "start \"\" \"" + documento + "\"";
        comando.Replace("/","\\",true);
        wxShell(_T(comando));
    }
    else {
        comando = "\"" + pdfreader + "\" \"" + documento + "\"";
        wxExecute(_T(comando));
    }
}


// MENU ZCODE  ******************************************************
// MENU - SEARCH
void MyFrame::OnZcodeVersion(wxCommandEvent &event){
    int id = event.GetId();
    switch (id)
    {
        case ID_ZcodeVersion5: 
            zcodeversion=".z5"; 
            zcodeswitch="-v5";
            break;
        case ID_ZcodeVersion8: 
            zcodeversion=".z8"; 
            zcodeswitch="-v8";            
            break;
    }
}
// Create Blb
void MyFrame::OnCreateBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    if (event.GetId() != ID_MakeAllBlb) OnClear();
    OnOutput("Creating blorb file...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString blcFile, blbFile;
    if (mainFile==""){
        blcFile = e->GetFilename();
        blbFile = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");
        blcFile = mainFile;
        blbFile = mainFile;
    }
    wxString wpath=wxPathOnly(blbFile);
    wxString mpath = wxGetCwd();
    wxSetWorkingDirectory(wpath);


    blcFile.Replace(".inf", ".blc", true);
    blbFile.Replace(".inf", "."+bext, true);
    wxString comando =  "\""+blc +"\""+" "+"\""+blcFile+"\""+" "+"\""+blbFile+"\"";

    //wxString comando =  blc +" "+blcFile+" "+blbFile;    
    wxArrayString output;
    if ( wxExecute(_T(comando), output) != 0 )
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("ERROR.\n");
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("Ok.\n");
        inform_error = false;
    }    
    wxSetWorkingDirectory(mpath);
}

void MyFrame::OnCreateRes (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    if (event.GetId() != ID_MakeAllBlb) OnClear();
    OnOutput("Creating Resources for blorb file...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString bresFile;
    if (mainFile==""){
        bresFile = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");
        bresFile = mainFile;
    }
    bresFile.Replace(".inf", "", true);
    wxString comando =  "\""+bres +"\""+" "+"\""+bresFile+"\"";
    wxArrayString output;
    if ( wxExecute(_T(comando), output) != 0 )
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }

        OnOutput("ERROR.\n");
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("Ok.\n");
        inform_error = false;
    }
}

// Make All Blb
void MyFrame::OnMakeAllBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnCreateRes(event); 
    if (inform_error) return;
    OnCompileUlx(event); 
    if (inform_error) return;
    OnCreateBlb(event);
}



// Run BLB
void MyFrame::OnRunBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput("Running blorb...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    
    // Running the MAIN FILE
    wxString nome;
    if (mainFile==""){
        nome = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");
        nome = mainFile;
    }
    nome.Replace(".inf", "."+bext, true);
    wxString comando =  pConfig->Read("GLULXINTERPRETER", _T("")) +" "+"\""+nome+"\"";
    OnOutput(comando+"\n");
    wxArrayString output;
    wxExecute(_T(comando));
}

// Run Ulx
// Should I Run the MAIN FILE
void MyFrame::OnRunUlx (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput("Running glulx...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());

    // Running the MAIN FILE
    wxString nome;
    if (mainFile==""){
        nome = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");
        nome = mainFile;
    }        
    nome.Replace(".inf", ".ulx", true);
    wxString comando =  pConfig->Read("GLULXINTERPRETER", _T("")) +" "+"\""+nome+"\"";
    OnOutput(comando+"\n");
    wxArrayString output;
    wxExecute(_T(comando));
}

// Compile ULX file
void MyFrame::OnCompileUlx (wxCommandEvent &event) {
    // Se non ho finetre aperte esco
    //PL: L'ho fixata io, ma non mi ricordo come
    //    probabilmente copiando quella per z-code
    if (auinotebook->GetPageCount()==0){ return; }

    // Azzero la console
    if (event.GetId() != ID_MakeAllBlb) OnClear();
    
    // AS: pulisco la console
    OnClear();
    
    // PL: Salvo tutto
    OnOutput("Saving all...\n");
    //wxCommandEvent subev;
    OnSaveAll(event);
    
    OnOutput("Compiling in ULX format...\n");
    wxArrayString output;

    // Recupero il nome del file da compilare
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome,ulx;
    if (mainFile==""){
        nome = e->GetFilename();
        ulx = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");
        nome = mainFile;
        ulx = mainFile;
    }
    ulx.Replace(".inf", ".ulx", true);
    wxString comando =  informCompiler +" -G "+libraryPath+" \""+nome+"\" "+"\""+ulx+"\"";

    //if ( wxExecute(_T("./inform.exe twocol.inf"), output) != 0 )
    if ( wxExecute(_T(comando), output) != 0 )
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            //wxPrintf(_T("\t%s\n"), output[n].c_str());
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }

        OnOutput("ERROR.\n");
        inform_error = true;

    }
    else
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            //wxPrintf(_T("\t%s\n"), output[n].c_str());
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("Ok.\n");
        inform_error = false;
    }
}

// Create ZBLB
void MyFrame::OnCreateZBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput("Creating Resources for z-blorb file...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString idFile;
    if (mainFile==""){
        idFile = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");
        idFile = mainFile;
    }
    idFile.Replace(".inf", "", true);
    wxString comando =  "\""+bres +"\" \""+idFile+".zres\" \""+idFile+".zblc\" 2del.bli";
    wxArrayString output;
    if ( wxExecute(_T(comando), output) != 0 )
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }

        OnOutput("ERROR.\n");
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("Ok.\n");
        inform_error = false;
    }
    if (inform_error) return;        

    OnCompile(event);
    if (inform_error) return;    

    OnOutput("Creating z-blorb file...\n");

    wxString wpath=wxPathOnly(idFile);
    wxString mpath = wxGetCwd();
    wxSetWorkingDirectory(wpath);

    comando =  "\""+blc +"\" \""+idFile+".zblc\" \""+idFile+".zblorb\"";

    //wxString comando =  blc +" "+blcFile+" "+blbFile;
    if ( wxExecute(_T(comando), output) != 0 )
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("ERROR.\n");
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("Ok.\n");
        inform_error = false;
    }
    wxSetWorkingDirectory(mpath);
}

// Run ZBLB
void MyFrame::OnRunZBlb (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput("Running z-blorb...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());

    // Running the MAIN FILE
    wxString nome;
    if (mainFile==""){
        nome = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");
        nome = mainFile;
    }
    nome.Replace(".inf", ".zblorb", true);
    wxString comando =  pConfig->Read("ZCODEINTERPRETER", _T("")) +" "+"\""+nome+"\"";
    OnOutput(comando+"\n");
    wxArrayString output;
    wxExecute(_T(comando));
}

// Run zcode
void MyFrame::OnRunZcode (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    OnClear();
    OnOutput("Running zcode...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome = e->GetFilename();
    if (mainFile=="") {
        nome = e->GetFilename();
    } else {
        OnOutput("Using MainFile: "+mainFile+"\n");
        nome = mainFile;
    }
    nome.Replace(".inf", zcodeversion, true);    
    wxString comando =  pConfig->Read("ZCODEINTERPRETER", _T("")) +" "+"\""+nome+"\"";
    OnOutput(comando+"\n");
    wxArrayString output; 
    wxExecute(_T(comando));
}

// Compile
void MyFrame::OnCompile (wxCommandEvent &event) {
    // Se non ho finetre aperte esco
    if (auinotebook->GetPageCount()==0){ return; }

    // Azzero la console
    if (event.GetId() != ID_CreateZBlb) OnClear();

    // AS: pulisco la console
    OnClear();
        
    // PL: Salvo tutto
    OnOutput("Saving all...\n");
    wxCommandEvent subev;
    OnSaveAll(subev);
        
    OnOutput("Compiling Z-Code...\n");
    wxArrayString output;
    
    // Recupero il nome del file da compilare
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome,zcode;    
    if (mainFile==""){
        nome = e->GetFilename();
        zcode = e->GetFilename();
    }
    else{
        OnOutput("Using MainFile: "+mainFile+"\n");        
        nome = mainFile;
        zcode = mainFile;        
    }
    zcode.Replace(".inf", zcodeversion, true);
    wxString comando =  informCompiler+" "+zcodeswitch+" "+libraryPath+" \""+nome+"\" "+" \""+zcode+"\" ";
    
    //if ( wxExecute(_T("./inform.exe twocol.inf"), output) != 0 )
    if ( wxExecute(_T(comando), output) != 0 )    
    {
        OnOutput(comando);
        OnOutput("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("ERROR.\n");
        inform_error = true;
    }
    else
    {
        OnOutput(comando);
        OnOutput("\n");        
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            OnOutput(output[n].c_str());
            OnOutput("\n");
        }
        OnOutput("Ok.\n");
        inform_error = false;
    }
}

// EVENTI SULL'ALBERO (TREE)
void MyFrame::OnSelChanged(wxTreeEvent &event)
{
    // Recupero il numero della posizione caret
    wxTreeItemId itemId = event.GetItem();
    MyTreeItemData *item = (MyTreeItemData *)tree->GetItemData(itemId);

    if ( item != NULL )
    {
        // Recupero l'edit corrente
        Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
        int start  = item->GetPosition();
        int end  = item->GetPosition()+item->GetLength();
        int linea = e->LineFromPosition(start);        
        e->GotoLine(linea);        
        e->SetSelection(start,end);        
        //OnOutput(wxString::Format("%d", item->GetPosition())+"\n");        
        //OnOutput(wxString::Format("%d", item->GetPosition())
        //+" "+"start["+wxString::Format("%d", start)+"]"
        //+" "+"end["+wxString::Format("%d", end)+"]\n");
    }
}

// AS: funzione che wrappa la appendText per il problema del readonly
void MyFrame::OnClear(){
    console->SetReadOnly(false);
    console->SetText("");
    console->SetReadOnly(true);    
}

// AS: funzione che wrappa la appendText per il problema del readonly
void MyFrame::OnOutput(wxString string) {
    console->SetReadOnly(false);    
    console->AppendText(string);
    console->SetReadOnly(true);    
}

// AS
void MyFrame::OnConsoleClicked(wxStyledTextEvent &event) {
    if (event.GetEventObject() == console) {
        // Recupero la riga selezionata
        int posizione = console->GetCurrentPos();       
        wxString risultato = console->GetCurLine(&posizione);
        // Controllo solo se contiene la stringa ERROR
        if (risultato.Contains(_T("Error")) || risultato.Contains(_T("Warning"))) {
            wxString file = risultato.Mid(0,risultato.Find('('));
            wxString riga = risultato.Mid(risultato.Find('(')+1,risultato.Find(')')-risultato.Find('(')-1);
            console->SetSelectionStart(0);
            console->SetSelectionEnd(0);
            console->Clear();
            
            // Se il file non è aperto, lo apro        
            if (!checkOpenFile(file)){
                wxString nome = file.Mid(file.Find('\\',true)+1,file.Length());
                LoadFile(file, nome);            
            }
            
            int pannelli = auinotebook->GetPageCount();
            for (int i=0; i<=pannelli-1 ;i++) {
                Edit* e = (Edit*) auinotebook->GetPage(i);
                wxString nome = e->GetFilename();
                if (file.Cmp(nome)==0){
                    e->GotoLine(wxAtoi(riga)-1);
                    int lineStart = e->PositionFromLine (e->GetCurrentLine());
                    int lineEnd = e->PositionFromLine (e->GetCurrentLine() + 1);
                    e->SetSelection (lineStart, lineEnd);
                    auinotebook->SetSelection(i);
                }
            }            
        }
    }   
}


// EVENTI SU NOTEBOOK
void MyFrame::OnNotebookPageChanged(wxAuiNotebookEvent& evt)
{
    OnUpdateTree();
}

void MyFrame::OnNotebookPageClose(wxAuiNotebookEvent& evt)
{   
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
    if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(Edit)))
    {
        Edit* e = (Edit*) ctrl->GetPage(evt.GetSelection());
        wxString titolo = ctrl->GetPageText(evt.GetSelection());
        if (e->Modified()) {
            int risposta = wxMessageBox (_("Text is not saved, save before closing?"), titolo,
            wxYES_NO | wxCANCEL | wxICON_QUESTION);

            if (risposta == wxYES) {
                e->SaveFile();
                if (e->Modified()) {
                    wxMessageBox (_("Text could not be saved!"), _("Close abort"),
                    wxOK | wxICON_EXCLAMATION);
                }
            }
            else if (risposta == wxCANCEL){
                evt.Veto();
            }
        }
    }
    tree->DeleteAllItems(); 
    OnClear();     
}

void MyFrame::OnExit (wxCommandEvent &event) {
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli ;i++){
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString titolo = auinotebook->GetPageText(i);
        if (e) {
            if (e->Modified()) {
                int risposta = wxMessageBox (_("Text is not saved, save before closing?"), titolo,
                    wxYES_NO | wxCANCEL | wxICON_QUESTION);
                if (risposta == wxYES) {
                    e->SaveFile();
                    if (e->Modified()) {
                        wxMessageBox (_("Text could not be saved!"), _("Close abort"),
                        wxOK | wxICON_EXCLAMATION);
                        continue;
                    }
                }
                else if (risposta == wxCANCEL){
                    return;
                }
            }
        }
    }
    Destroy();
}

void MyFrame::OnClose (wxCloseEvent &event) {
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli ;i++){
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString titolo = auinotebook->GetPageText(i);
        if (e) {
            if (e->Modified()) {
                int risposta = wxMessageBox (_("Text is not saved, save before closing?"), titolo,
                    wxYES_NO | wxCANCEL | wxICON_QUESTION);
                if (risposta == wxYES) {
                    e->SaveFile();
                    if (e->Modified()) {
                        wxMessageBox (_("Text could not be saved!"), _("Close abort"),
                        wxOK | wxICON_EXCLAMATION);
                        continue;
                    }
                }
                else if (risposta == wxCANCEL){
                    return;
                }
            }
        }
    }
    OnUpdateTree();   
    Destroy();    
}
 
// Tutti gli eventi di EDIT vengono gestiti dal componente Stc 
void MyFrame::OnEdit (wxCommandEvent &event) {
    
    int id = event.GetId();
//    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION);
    switch (id)
    {
        case myID_LINENUMBER: showLineNumber=event.IsChecked(); break;
        case myID_WRAPMODEON: wrapMode=event.IsChecked(); break;
        case myID_AUTOCOMPON: autoCompleteSwitch=event.IsChecked(); break;
        case myID_HOTKEYSON:  hotkeysSwitch=event.IsChecked(); break;
    }    
    
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli ;i++){
        Edit* e = (Edit*) auinotebook->GetPage(i);
        if (e) {
            e->ProcessEvent (event);
        }
    }       
}
 
bool MyFrame::checkOpenFile(wxString path)
{
    bool result = false;
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli-1 ;i++) {
        Edit* e = (Edit*) auinotebook->GetPage(i);
        wxString nome = e->GetFilename();
        if (path.Cmp(nome)==0){
            result = true;
        }        
    }    
    return result;
} 

static int CompareStrings(const wxString& first, const wxString& second)
{
    return first.CmpNoCase(second);
}

void MyFrame::setNewStc(Edit* stc) {
    // Impostazioni del nuovo stc : PL
    if (showLineNumber){
        stc->SetMarginWidth (0, stc->TextWidth (wxSTC_STYLE_LINENUMBER, _T("_999999")));
    }
    else{
        stc->SetMarginWidth (0, 0);
    }
    if (wrapMode){
        stc->SetWrapMode (wxSTC_WRAP_WORD);
    }
    else{
        stc->SetWrapMode (wxSTC_WRAP_NONE);
    }

    stc->SetCaretLineVisible(true);
    stc->SetCaretLineBackground(wxColour(152,248,248,wxALPHA_OPAQUE));
    stc->AutoCompSetIgnoreCase(true);
    stc->AutoCompSetAutoHide(true);

    //wxString languagename = stc->QueryLanguage();
    if (stc->QueryLanguage() != "INFORM") {
        stc->ClearHotkeys();
        stc->SetAutoComplete(false, false);
        stc->SetHotkeys(false, false);   
        return;     
    }
    bool bCont;
    long dummy;
    wxString str, s;
    // Recupero le hotkey
    stc->ClearHotkeys();
    pConfig->SetPath(_T("/HOTKEYS"));
    bCont = pConfig->GetFirstEntry(str, dummy);
    while(bCont){
        s = pConfig->Read(_T(str),_T(""));
        stc->AddHotkey(s);
        //OnOutput(_T(str)+" = "+_T(s)+" ;  ");
        bCont = pConfig->GetNextEntry(str, dummy);
    }   
    //wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION); 
    
    // Recupero tutte le wordlist per AUTOCOMPLETION dal file di configurazione
    // Recupero le liste separatamente, aggiungendo le parole sia alle varie
    // liste per la syntax highlighting che all'arraty wlarray
    wxArrayString wlarray;
    pConfig->SetPath(_T("/STATEMENTS"));
    wxString statlist = "";
    bCont = pConfig->GetFirstEntry(str, dummy);
    while(bCont){
        s = pConfig->Read(_T(str),_T(""));
        statlist = statlist + s + " ";
        wlarray.Add(s);
        bCont = pConfig->GetNextEntry(str, dummy);
    }
    pConfig->SetPath(_T("/DIRECTIVES"));
    wxString direclist = "";
    bCont = pConfig->GetFirstEntry(str, dummy);
    while(bCont){
        s = pConfig->Read(_T(str),_T(""));
        direclist = direclist + s + " ";
        wlarray.Add(s);
        bCont = pConfig->GetNextEntry(str, dummy);
    }
    pConfig->SetPath(_T("/OTHERKEYWORDS"));
    wxString otherlist = "";
    bCont = pConfig->GetFirstEntry(str, dummy);
    while(bCont){
        s = pConfig->Read(_T(str),_T(""));
        otherlist = otherlist + s + " ";
        wlarray.Add(s);
        bCont = pConfig->GetNextEntry(str, dummy);
    }    
    pConfig->SetPath(_T("/"));
    //wxString wordlist = statlist+direclist+otherlist;
    // Aggiungo la roba del progetto
    if ( mainFile != "" ) {
        for (size_t i = 0; i<projclasses.GetCount(); i++) {
            direclist = direclist + projclasses[i] + " ";
            wlarray.Add(projclasses[i]);
        }
        for (size_t i = 0; i<projkeywords.GetCount(); i++) {
            otherlist = otherlist + projkeywords[i] + " ";
            wlarray.Add(projkeywords[i]);
        }            
    }
    //Ordino l'array e lo copio nella lista per l'autocompletion
    wlarray.Sort(CompareStrings);
    wxString wordlist = "";
    for (size_t i = 0; i<wlarray.GetCount(); i++) wordlist = wordlist + wlarray[i] + " ";
    wlarray.Clear();
    stc->SetWordlist(wordlist);
    stc->SetKeyWords (mySTC_TYPE_DEFAULT, statlist + direclist + otherlist);
    //stc->SetKeyWords (1, direclist);
    //stc->SetKeyWords (2, otherlist);   
    stc->SetAutoCompleteNumber(autoCompleteNumber);
    stc->SetAutoComplete(autoCompleteSwitch, true);   
    stc->SetHotkeys(hotkeysSwitch, true);

    /*OnClear();
    OnOutput(wordlist);
    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION); */
    
}
    
 
void MyFrame::LoadFile(wxString path, wxString name)
{
    if (checkOpenFile(path)) return;
    Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER, path);
    stc->LoadFile (path);
    
    setNewStc(stc);
   
    OnClear();

    wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
    auinotebook->AddPage(stc  , name, true , page_bmp);
    OnUpdateTree();
}
 
void MyFrame::OnLoadFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* fd = new wxFileDialog(this, "Open File","","","Inform Source Files (*.inf;*.h)|*.inf;*.h|All Files (*.*)|*.*",
    wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,"filedlg");
    wxString path,name;
    if (fd->ShowModal() == wxID_OK ){
        path = fd->GetPath();
        name = fd->GetFilename();
        
        // Check if file is already opened
        if (checkOpenFile(path)){
           wxMessageBox (_("File is already opened."),_("Warning"), wxOK | wxICON_WARNING);    
           return;
        }
        
        Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER, path);
        stc->LoadFile (path);
        
        setNewStc(stc);
        
        OnClear();
        
        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
        auinotebook->AddPage(stc  , name, true , page_bmp);
        //auinotebook->SetSelection(auinotebook->GetSelection()+1);
        OnUpdateTree();        
    }   
} 

// MENU PROJECT

void MyFrame::OnOpenProject(wxCommandEvent& WXUNUSED(event)) {
    wxFileDialog* fd = new wxFileDialog(this, "Open Wide Project","","","Wide Project Files (*.wpf)|*.wpf|All Files (*.*)|*.*",
    wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,"filedlg");    
    if (fd->ShowModal() == wxID_OK ){
        wxString path = fd->GetPath();
        wxString name = fd->GetFilename();        
        OnClear();
        mainFile = "";
        projclasses.Empty();
        projkeywords.Empty();
        
        wxFileConfig* projfile = new wxFileConfig(
        NOMEAPPLICAZIONE, NOMEAPPLICAZIONE,
        path, "", wxCONFIG_USE_RELATIVE_PATH, wxConvUTF8);
        //wxConfigBase::Set(projfile);
        //projfile->SetPath(_T("/")); 

        wxString s, str;
        bool bCont; long dummy;
        int zc = 0;
        zc = projfile->Read(_T("ZCODEVERSION"), 1l);
        switch (zc) {
          case 8:
            zcodeversion=".z8";
            zcodeswitch="-v8";
            zcodemenu->Check(ID_ZcodeVersion8, true);
            break;
          case 5:
            zcodeversion=".z5";
            zcodeswitch="-v5";
            zcodemenu->Check(ID_ZcodeVersion5, true);
            break;
        }        
        projfile->SetPath(_T("/CLASSES"));
        bCont = projfile->GetFirstEntry(str, dummy);
        while(bCont){
          s = projfile->Read(_T(str),_T(""));
          projclasses.Add(s);
          bCont = projfile->GetNextEntry(str, dummy);
        }        
        projfile->SetPath(_T("/KEYWORDS"));
        bCont = projfile->GetFirstEntry(str, dummy);
        while(bCont){
          s = projfile->Read(_T(str),_T(""));
          projkeywords.Add(s);
          bCont = projfile->GetNextEntry(str, dummy);
        }        
        projfile->SetPath(_T("/FILES"));
        bCont = projfile->GetFirstEntry(str, dummy);
        while(bCont){
          s = projfile->Read(_T(str),_T(""));
          str = path.Mid(0,path.Find('\\',true)+1) + s;
          LoadFile(str,s);
          if (mainFile == "") {
                mainFile = str;
                OnOutput("Using Main file ["+mainFile+"] ");
          }
          bCont = projfile->GetNextEntry(str, dummy);
        }
        //wxMessageBox (_(path), _("Close abort"),  wxOK | wxICON_EXCLAMATION);

    }    
    //wxConfigBase::Set(pConfig);    
}
  


void MyFrame::OnSaveProject(wxCommandEvent& WXUNUSED(event))
{
}

void MyFrame::OnCloseProject(wxCommandEvent& WXUNUSED(event))
{
    projclasses.Empty();
    projkeywords.Empty();
    if (mainFile!=""){
        wxMessageBox (mainFile, _("Remove Main File"),  wxOK | wxICON_INFORMATION);            
        mainFile="";
        OnClear();
        OnOutput("Main file, classes and keywords removed.");        
    }
    else{
        wxMessageBox (_("No Project opened."),_("Warning"), wxOK | wxICON_WARNING);    
    }
}


void MyFrame::OnLicense(wxCommandEvent& WXUNUSED(event))
{

    wxAboutDialogInfo info;
    info.SetName(_(NOMEAPPLICAZIONE));
    info.SetVersion(_(VERSIONE));
    info.SetCopyright(_T("(C) 2008 - Alessandro Schillaci"));
    info.SetWebSite(_T("http://wide.berlios.de/"), _T("Home page"));
    info.SetDescription(wxString::FromAscii(
"GNU GPL LICENSE\n"
"===============\n"
"\n"
"  This program is free software; you can redistribute it and/or modify\n"
"  it under the terms of the GNU General Public License as published by\n"
"  the Free Software Foundation; either version 3 of the License, or\n"
"  (at your option) any later version.\n"
"\n"
"  This program is distributed in the hope that it will be useful,\n"
"  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"  GNU General Public License for more details.\n"
"\n"
"  You should have received a copy of the GNU General Public License\n"
"  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
    ));
    wxAboutBox(info);
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    
    wxAboutDialogInfo info;
    info.SetName(_(NOMEAPPLICAZIONE));
    info.SetVersion(_(VERSIONE));
    info.SetDescription(_(DESCRIZIONE));
    info.SetCopyright(_T("(C) 2008 - schillacia@users.berlios.de"));
    info.SetWebSite(_T("http://wide.berlios.de/"), _T("Home page"));
    info.AddDeveloper(_T("Alessandro Schillaci"));
    info.AddDeveloper(_T("Paolo Lucchesi"));
    wxAboutBox(info);    
}
 
// Funzione che aggiunge un nodo alla root con i valori relativi
void MyFrame::OnUpdateTreeRegularExpression(wxString text, wxTreeItemId root, wxString nome, wxString pattern_global, bool keepquote){
    int contatore=0;    
    wxRegEx re;
    if (!re.Compile(pattern_global, wxRE_ICASE|wxRE_NEWLINE) ){
        OnOutput("Errore nella RE");
        return;
    }
    wxArrayTreeItemIds items;
    items.Add(tree->AppendItem(root, nome, 0));
    
    while ( re.Matches(text) ){
        size_t start, len, n=0;
        int i=0;
        while (re.GetMatch(&start, &len, n)){
            n+=1;
        }
        if (text[start-1]=='\"' && keepquote) {start--; len++; }
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, text.Mid(start,len),1,-1, new MyTreeItemData(start+contatore,len));
        i++;
        text = text.Mid(start+len,text.Len());
        contatore=contatore + start+len;
    }        
}
 
void MyFrame::OnUpdateTree()
{
    if (auinotebook->GetPageCount()==0){
        tree->DeleteAllItems(); 
        return;
    }
    
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    
    wxTreeItemId wroot = tree->GetRootItem();
    wxTreeItemIdValue ck, ck2;
    wxTreeItemId cid, cen;
    wxString cname;
    wxArrayString memo;
    cid = tree->GetFirstChild(wroot, ck);
    while (cid.IsOk()) {
        if (tree->ItemHasChildren(cid)) {
          if (tree->IsExpanded(cid)) cname = "+" + tree->GetItemText(cid);
           else                      cname = "-" + tree->GetItemText(cid);
          memo.Add(cname);
    }
        //OnOutput(cname);
        cid = tree->GetNextChild(wroot, ck);
    }
    cid = tree->GetFirstVisibleItem();
    wxString firstvis = tree->GetItemText(cid);
    
    wxString text = e->GetText();
    tree->DeleteAllItems();    
    wxArrayTreeItemIds items;
    wxTreeItemId root = tree->AddRoot(wxT("Object Tree"), 0);


    if (showObjects) OnUpdateTreeRegularExpression(text, root, "Object", "\n+[ \t\f\v]*Object[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*\"?([A-Za-z0-9_]+)\"?",true);
    if (showProject) {
        for (size_t i = 0; i<projclasses.GetCount(); i++) {
            wxString regexp = "\n+[ \t\f\v]*"+projclasses[i]+"[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*\"?([A-Za-z0-9_]+\"?)";
            OnUpdateTreeRegularExpression(text, root, projclasses[i], regexp, true);
        }        
    }
    if (showGlobals) OnUpdateTreeRegularExpression(text, root, "Global", "\n+[ \t\f\v]*Global[ \t\n\r\f\v]+([A-Za-z0-9_]+)", false);
    if (showConstants) OnUpdateTreeRegularExpression(text, root, "Constant", "\n+[ \t\f\v]*Constant[ \t\n\r\f\v]+([A-Za-z0-9_]+)", false);
    if (showFunctions) OnUpdateTreeRegularExpression(text, root, "Function", "\n+[ \t\n\r\f\v]*\\[[ \t\n\r\f\v]*([A-Za-z0-9_]+).*;", false);
    if (showClasses) OnUpdateTreeRegularExpression(text, root, "Class", "\n+[ \t\f\v]*Class[ \t\n\r\f\v]+([A-Za-z0-9_]+)", false);
    //if (showIncludes) OnUpdateTreeRegularExpression(text, root, "Include", "\n+[ \t\f\v]*Include[ \t\f\v]*\"([A-Za-z0-9_]+)(\x2eh)?\"", false);
    if (showIncludes) OnUpdateTreeRegularExpression(text, root, "Include", "\n+[ \t\f\v]*Include[ \t\f\v]*\"([A-Za-z0-9_]+)", false);
    if (showVerbs) OnUpdateTreeRegularExpression(text, root, "Verb", "\n+[ \t\f\v]*Verb[ \t\f\v]+\'([A-Za-z0-9_]+)'", false);

    if (expandAllNodes) {
        tree->ExpandAll();                
    }
    else{
        tree->Expand(root);     
    }
    
    wroot = tree->GetRootItem();
    cid = tree->GetFirstChild(wroot, ck);
    while (cid.IsOk()) {
        cname = tree->GetItemText(cid);
        for (size_t i = 0; i<memo.GetCount(); i++) {
            if (memo[i] == "+" + cname) { tree->Expand(cid); break; }
            if (memo[i] == "-" + cname) { tree->Collapse(cid); break; }
        }
        cid = tree->GetNextChild(wroot, ck);
    }
    
    // Scroll all'inizio dell'albero
    tree->ScrollTo(root);
    cid = tree->GetFirstChild(wroot, ck);
    bool brf = false;
    while (cid.IsOk()) {
        if ((tree->GetItemText(cid)) == firstvis) { tree->ScrollTo(cid); break; }
        cen = tree->GetFirstChild(cid, ck2);
        while (cen.IsOk()) {
            if ((tree->GetItemText(cen)) == firstvis) { tree->ScrollTo(cen); brf = true; break; }
            cen = tree->GetNextChild(cid, ck2);
        }
        if (brf) break;
        cid = tree->GetNextChild(wroot, ck);
    }
    tree->SetScrollPos(wxHORIZONTAL, 0, true);
    
}
 
 
  
 

// FUNZIONI CREAZIONE CONTROLLI

// NOTEBOOK
wxAuiNotebook* MyFrame::CreateNotebook()
{
   // create the notebook off-window to avoid flicker
   wxSize client_size = GetClientSize();
   
   wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
   return ctrl;
}


// MENUBAR |--------------------------------------------------------------------
wxMenuBar* MyFrame::CreateMenuBar()
{
    wxMenuBar* mb = new wxMenuBar;

    // FILE MENU ----------------------------------------------------------------------------
    wxMenu* file = new wxMenu;    
    // FILE::NEW
    wxMenuItem *new_file = new wxMenuItem(file, ID_NewFile, _("&"+MENU_FILE_NEWFILE+"\tCtrl+N"));
    new_file->SetBitmap(new_xpm);
    file->Append(new_file);    
    
    // FILE::LOAD
    wxMenuItem *load_file = new wxMenuItem(file, ID_LoadFile, _("&"+MENU_FILE_OPENFILE+"\tCtrl+O"));
    load_file->SetBitmap(fileopen_xpm);
    file->Append(load_file);

    // FILE::SAVE
    wxMenuItem *save_file = new wxMenuItem(file, ID_Save_File, _("&"+MENU_FILE_SAVEFILE+"\tCtrl+S"));
    save_file->SetBitmap(filesave_xpm);
    file->Append(save_file);
    
    //FILE::SAVEALL   //PL
    wxMenuItem *save_all = new wxMenuItem(file, ID_Save_All, _("&"+MENU_FILE_SAVEALL+"\tCtrl+Shift+S"));
    save_all->SetBitmap(filesaveall_xpm);
    file->Append(save_all);
    
    file->AppendSeparator();

    //FILE::NEXTPAGE   //PL
    wxMenuItem *next_page = new wxMenuItem(file, ID_NextPage, _("&"+MENU_FILE_NEXTTAB+"\tCtrl+F6"));
    next_page->SetBitmap(forward_xpm);
    file->Append(next_page);
    
    //FILE::PREVIOUSPAGE   //PL
    wxMenuItem *previous_page = new wxMenuItem(file, ID_PreviousPage, _("&"+MENU_FILE_PREVIOUSTAB+"\tCtrl+F5"));
    previous_page->SetBitmap(back_xpm);
    file->Append(previous_page);

    file->AppendSeparator();
    
    wxMenuItem *quit = new wxMenuItem(file, ID_Exit, _("&"+MENU_FILE_QUIT+"\tCtrl+Q"));
    quit->SetBitmap(quit_xpm);
    file->Append(quit);

    // EDIT MENU -------------------------------------------------------------------------
    wxMenu* edit = new wxMenu;
    // EDIT::COPY
    wxMenuItem *copy = new wxMenuItem(edit, wxID_COPY, _("&"+MENU_EDIT_COPY+"\tCtrl+C"));
    copy->SetBitmap(copy_xpm);
    edit->Append(copy);

    // EDIT::CUT
    wxMenuItem *cut = new wxMenuItem(edit, wxID_CUT, _("&"+MENU_EDIT_CUT+"\tCtrl+X"));
    cut->SetBitmap(cut_xpm);
    edit->Append(cut);
    
    // EDIT::PASTE
    wxMenuItem *paste = new wxMenuItem(edit, wxID_PASTE, _("&"+MENU_EDIT_PASTE+"\tCtrl+V"));
    paste->SetBitmap(paste_xpm);
    edit->Append(paste);    
    edit->AppendSeparator();

    edit->Append(wxID_UNDO, _("&"+MENU_EDIT_UNDO+"\tCtrl+Z"));
    edit->Append(wxID_REDO, _("&"+MENU_EDIT_REDO+"\tCtrl+Y"));
    edit->AppendSeparator();
    
    // EDIT::INDENT
    wxMenuItem *indent_r = new wxMenuItem(edit, myID_INDENTINC, _("&"+MENU_EDIT_INDENT+"\tTab"));
    //indent_r->SetBitmap(forward_xpm);
    edit->Append(indent_r);

    // EDIT::UNINDENT
    wxMenuItem *indent_l = new wxMenuItem(edit, myID_INDENTRED, _("&"+MENU_EDIT_UNINDENT+"\tShift+Tab"));
    //indent_l->SetBitmap(back_xpm);
    edit->Append(indent_l);
    edit->AppendSeparator();

    // EDIT::COMMENT
    wxMenuItem *comment = new wxMenuItem(edit, ID_Comment, _("&"+MENU_EDIT_COMMENT+"\tCtrl+Shift+C"));
    //indent_l->SetBitmap(back_xpm);
    edit->Append(comment);

    // EDIT::UNCOMMENT
    wxMenuItem *uncomment = new wxMenuItem(edit, ID_Uncomment, _("&"+MENU_EDIT_UNCOMMENT+"\tCtrl+Shift+U"));
    //indent_l->SetBitmap(back_xpm);
    edit->Append(uncomment);

    // SEARCH MENU
    wxMenu* search = new wxMenu;
    search->Append (ID_Find, _("&Find\tF3"));
    search->Append (ID_FindBack, _("&Find Backwards\tShift+F3"));   //PL
    search->Append (ID_FindReplace, _("Find and &replace\tCtrl+F3"));
    search->Append (ID_FindObjectLocal, _("&Find Object\tF4"));
    search->Append (ID_FindObjectGlobal, _("&Find Object in files\tShift+F4"));
    search->AppendSeparator();
    search->Append(ID_GotoLine, _("&Goto line\tCtrl+L"));
    search->AppendSeparator();
    search->Append(ID_NextMarker, _("&Next Marker\tF2"));
    wxMenuItem *marker = new wxMenuItem(search, ID_ToggleMarker, _("&Toggle Marker\tCtrl+F2"));
    
    marker->SetBitmap(addbookm_xpm);
    search->Append(marker);        
    //search->Append(ID_ToggleMarker, _("&Toggle Marker\tCtrl+F2")); 
    search->Append(ID_ResetMarkers, _("&Reset Markers\tCtrl+Shift+F2"));

    // PROJECT MENU
    wxMenu* project = new wxMenu;
    project->Append (ID_OpenProject, _("&Open Project\tCtrl+P"));
    project->Append (ID_SaveProject, _("&Save Project"));
    project->Append (ID_CloseProject, _("&Close Project"));
    
    
    // ZCODE MENU
    wxMenu* zcode = new wxMenu;
    zcode->Append (ID_Compile, _("&Compile Zcode\tF8"));
    zcode->Append (ID_RunZcode, _("&Run Zcode\tCtrl+F8"));
    zcode->AppendSeparator();
    zcode->AppendRadioItem (ID_ZcodeVersion5, _("Zcode version 5"));
    zcode->AppendRadioItem (ID_ZcodeVersion8, _("Zcode version 8"));
    zcode->AppendSeparator();
    zcode->Append (ID_CreateZBlb, _("Create Zcode &blorb\tCtrl+Shift+F8"));
    zcode->Append (ID_RunZBlb, _("R&un Zcode blorb\tShift+F8"));    
    zcodemenu=zcode;
    
    
    // GLULX MENU
    wxMenu* glulx = new wxMenu;
    glulx->Append (ID_CompileUlx, _("&Compile Ulx\tF9"));            
    glulx->Append (ID_RunUlx, _("&Run Ulx\tCtrl+F9"));
    glulx->AppendSeparator();
    glulx->Append (ID_CreateRes, _("&Create Resources\tF10"));
    glulx->Append (ID_CreateBlb, _("&Create Blorb File\tCtrl+F10"));
    glulx->Append (ID_MakeAllBlb, _("&Build All Blorb File\tF11"));
    glulx->Append (ID_RunBlb, _("&Run Blorb\tCtrl+F11"));


    // OPTION MENU
    wxMenu* option = new wxMenu;
    option->AppendCheckItem (myID_LINENUMBER, _("Show line &numbers"));
    option->AppendCheckItem (myID_WRAPMODEON, _("&Wrap mode"));    
    option->AppendCheckItem (myID_AUTOCOMPON, _("&Autocomplete"));
    option->AppendCheckItem (myID_HOTKEYSON,  _("&Hotkeys Replacing"));

    
    
    option->Check(myID_LINENUMBER, showLineNumber);
    option->Check(myID_WRAPMODEON, wrapMode);
    option->Check(myID_AUTOCOMPON, autoCompleteSwitch);
    option->Check(myID_HOTKEYSON,  hotkeysSwitch);
    

    // OBJECT TREE MENU
    wxMenu* otree = new wxMenu;

    otree->Append (ID_RefreshTree, _("Refresh Tree\tENTER"));
    otree->AppendCheckItem (ID_ExpandAllAlways, _("Expand always all nodes"));
    otree->AppendSeparator();
    otree->AppendCheckItem (ID_ShowObject, _("Show Objects"));
    otree->AppendCheckItem (ID_ShowProject, _("Show Project Defs"));
    otree->AppendCheckItem (ID_ShowGlobal, _("Show Globals"));
    otree->AppendCheckItem (ID_ShowConstant, _("Show Constants"));
    otree->AppendCheckItem (ID_ShowFunction, _("Show Functions"));
    otree->AppendCheckItem (ID_ShowClass, _("Show Classes"));
    otree->AppendCheckItem (ID_ShowInclude, _("Show Includes"));
    otree->AppendCheckItem (ID_ShowVerb, _("Show Verbs"));
    
    // Load configuration from file    
    otree->Check(ID_ExpandAllAlways, (pConfig->Read(_T("OBJECT_TREE_EXPAND_ALL_NODES"), 1l) != 0) );
    otree->Check(ID_ShowObject, (pConfig->Read(_T("OBJECT_TREE_SHOW_OBJECTS"), 1l) != 0) );
    otree->Check(ID_ShowProject, (pConfig->Read(_T("OBJECT_TREE_SHOW_PROJECT"), 1l) != 0) );
    otree->Check(ID_ShowGlobal, (pConfig->Read(_T("OBJECT_TREE_SHOW_GLOBALS"), 1l) != 0) );
    otree->Check(ID_ShowConstant, (pConfig->Read(_T("OBJECT_TREE_SHOW_CONSTANTS"), 1l) != 0) );    
    otree->Check(ID_ShowFunction, (pConfig->Read(_T("OBJECT_TREE_SHOW_FUNCTIONS"), 1l) != 0) );
    otree->Check(ID_ShowClass, (pConfig->Read(_T("OBJECT_TREE_SHOW_CLASSES"), 1l) != 0) );
    otree->Check(ID_ShowInclude, (pConfig->Read(_T("OBJECT_TREE_SHOW_INCLUDES"), 1l) != 0) );
    otree->Check(ID_ShowVerb, (pConfig->Read(_T("OBJECT_TREE_SHOW_VERBS"), 1l) != 0) );

    wxMenu* docs = new wxMenu;
    if (pConfig->Read("PDF1_NAME", _T(""))!=""){docs->Append(ID_Doc1, pConfig->Read("PDF1_NAME", _T("")));}
    if (pConfig->Read("PDF2_NAME", _T(""))!=""){docs->Append(ID_Doc2, pConfig->Read("PDF2_NAME", _T("")));}
    if (pConfig->Read("PDF3_NAME", _T(""))!=""){docs->Append(ID_Doc3, pConfig->Read("PDF3_NAME", _T("")));}
    if (pConfig->Read("PDF4_NAME", _T(""))!=""){docs->Append(ID_Doc4, pConfig->Read("PDF4_NAME", _T("")));}
    if (pConfig->Read("PDF5_NAME", _T(""))!=""){docs->Append(ID_Doc5, pConfig->Read("PDF5_NAME", _T("")));}
    if (pConfig->Read("PDF6_NAME", _T(""))!=""){docs->Append(ID_Doc6, pConfig->Read("PDF6_NAME", _T("")));}
    if (pConfig->Read("PDF7_NAME", _T(""))!=""){docs->Append(ID_Doc7, pConfig->Read("PDF7_NAME", _T("")));}
    if (pConfig->Read("PDF8_NAME", _T(""))!=""){docs->Append(ID_Doc8, pConfig->Read("PDF8_NAME", _T("")));}
    


    wxMenu* help = new wxMenu;
    help->Append(ID_License, _("License"));    
    help->AppendSeparator();
    
    // HELP::ABOUT
    wxMenuItem *about = new wxMenuItem(help, ID_About, _("About Wide"));
    about->SetBitmap(help_xpm);
    help->Append(about);
    
    mb->Append(file, _(MENU_FILE));
    mb->Append(edit, _(MENU_EDIT));
    mb->Append(search, _("Search"));        
    mb->Append(project , _("Project"));            
    mb->Append(zcode, _("ZCode"));
    mb->Append(glulx, _("Glulx"));    
    mb->Append(otree, _("Object Tree"));
    mb->Append(option, _("Options"));
    mb->Append(docs, _("Documents"));        
    mb->Append(help, _("Help"));    
    
    return mb;
}
 
 
// ALBERO  |--------------------------------------------------------------------
wxTreeCtrl* MyFrame::CreateTreeCtrl()
{
    wxTreeCtrl* tree = new wxTreeCtrl(this, wxID_ANY,
                                      wxPoint(0,0), wxSize(160,250),
                                      wxTR_DEFAULT_STYLE | wxSIMPLE_BORDER);
                                      
    wxImageList* imglist = new wxImageList(16, 16, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    tree->AssignImageList(imglist);       
    return tree;
}

// TOOL BAR
wxToolBar* MyFrame::CreateToolBarCtrl()
{
    wxToolBar* tb2 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER);
    tb2->SetToolBitmapSize(wxSize(16,16));
   
    tb2->AddTool(ID_LoadFile, fileopen_xpm,"Open File");
    tb2->AddTool(ID_NewFile, new_xpm,"New File");    
    tb2->AddTool(ID_Save_File, filesave_xpm, "Save File");
    tb2->AddTool(103, filesaveas_xpm, "Save File as...");
    tb2->AddTool(ID_Save_All, filesaveall_xpm, "Save All");
    tb2->AddSeparator(); 
    
    tb2->AddTool(ID_PreviousPage, back_xpm,"Previous Tab");
    tb2->AddTool(ID_NextPage, forward_xpm,"Next Tab");
    tb2->AddSeparator();

    tb2->AddTool(wxID_UNDO, undo_xpm,"Undo");
    tb2->AddTool(wxID_REDO, redo_xpm,"Redo");
    tb2->AddSeparator(); 
    
    tb2->AddTool(wxID_COPY, copy_xpm,"Copy");
    tb2->AddTool(wxID_CUT, cut_xpm,"Cut");
    tb2->AddTool(wxID_PASTE, paste_xpm,"Paste");
    tb2->AddTool(ID_FindBack, findback_xpm,"Find Backwards");   //PL
    tb2->AddTool(ID_Find, find_xpm,"Find");    //PL
    tb2->AddTool(108, findrepl_xpm,"Find and Replace");
    tb2->AddTool(ID_FindObjectLocal, findobject_xpm,"Find Object");    //PL
    tb2->AddTool(ID_FindObjectGlobal, findobjectg_xpm,"Find Object");    //PL    
    tb2->AddSeparator();
    
    tb2->AddTool(ID_Compile, compilezcode_xpm, "Compile ZCode");
    tb2->AddTool(ID_RunZcode, exezcode_xpm, "Execute ZCode");
    tb2->AddSeparator();
    
    tb2->AddTool(ID_CompileUlx, compileulx_xpm, "Compile Glulx");
    tb2->AddTool(ID_RunUlx, exeglulx_xpm, "Execute Glulx");        
    tb2->AddSeparator();
    
    tb2->AddTool(ID_About, help_xpm, "About");
    tb2->AddTool(ID_Exit, quit_xpm, "Quit the program");
    tb2->Realize();
    
    return tb2;
}

  DECLARE_APP(MyApp);
  IMPLEMENT_APP(MyApp);
  
