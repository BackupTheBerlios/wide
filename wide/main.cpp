//////////////////////////////////////////////////////////////////////////////
// File:        main.cpp
// Purpose:     Wide Inform 6 Editor
// Author:      silver.slade@tiscali.it
// Licence:     GNU GPL
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

  #include "wx/tipdlg.h" // Gestione tips
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
  #include "images/quit.xpm"     
  #include "images/help.xpm"                        
  #include "images/copy.xpm"                      
  #include "images/cut.xpm"                      
  #include "images/paste.xpm"                       
  #include "images/new.xpm"           
  #include "images/tipicon.xpm"       
  #include "images/addbookm.xpm"       
  #include "images/forward.xpm"       
//  #include "images/back.xpm"             
                      


// COSTANTI
  #define SEP " - "
  #define VERSIONE "0.9 beta"
  #define BUILD " (build 20080320) "
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
        ID_Licence,
        ID_Support,
        ID_Save_File,
        ID_Save_All,        //PL
        ID_NB_Close,
        ID_ShowObject,
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
        ID_ShowTips,
        ID_NextMarker,
        ID_ToggleMarker,
        ID_RefreshTree,
        ID_Compile,
        ID_CompileUlx,                
        ID_RunZcode,
        ID_RunUlx,  
        ID_ZcodeVersion5,
        ID_ZcodeVersion8,        
        ID_Doc1,            
        ID_Doc2,            
        ID_Doc3,                            
        ID_Doc4,    
        ID_Doc5,            
        ID_Tip,              
        ID_OpenProject,           
        ID_SaveProject,         
        ID_CloseProject,
        ID_CreateBlb,    
        ID_CreateRes,                               
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
 
     // create several text controls                                        
     console = new wxTextCtrl(this, -1, _(""),
                      wxDefaultPosition, wxSize(100,100),
                      wxNO_BORDER | wxTE_MULTILINE);
    console->SetEditable(false);                                        
                                        
//     wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
//                      wxDefaultPosition, wxSize(200,150),
//                      wxNO_BORDER | wxTE_MULTILINE);
         
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
     
     // Show tips
     if (showTips) ShowTip();
     

   }
 
   ~MyFrame()
   {
     SaveConfiguration();
     // deinitialize the frame manager
     m_mgr.UnInit();
   }
 
 // Funzioni
 private:
    // Show tips
    void ShowTip();
    
    // FILE MENU
    void OnNewFile(wxCommandEvent& evt);    
    void OnLoadFile(wxCommandEvent& evt);
    void LoadFile(wxString path, wxString name);    
    void OnSaveFile(wxCommandEvent &event); 
    void OnSaveAll(wxCommandEvent &event);       //PL
    void OnExit(wxCommandEvent& evt);

    // MENU EDIT
    
                
    // MENU ABOUT
    void OnAbout(wxCommandEvent& evt);
    void OnLicence(wxCommandEvent& evt);
    void OnSupport(wxCommandEvent& evt);
        
    void OnOptions(wxCommandEvent &event);   
    void LoadConfiguration();   
    void SaveConfiguration();
   
    // Search menu
    void GotoLine(wxCommandEvent &event);   
    void Search(wxCommandEvent &event);
    void SearchBack(wxCommandEvent &event);   //PL
    void SearchReplace(wxCommandEvent &event);   
    void NextMarker(wxCommandEvent &event);
    void ToggleMarker(wxCommandEvent &event);
   
    // Tree
    void OnUpdateTree();   
    void OnRefreshTree(wxCommandEvent &event);
    void OnUpdateTreeRegularExpression(wxString text, wxTreeItemId root, wxString nome, wxChar *pattern);
    
    // Metodi lanciati su stc    
    void OnEdit (wxCommandEvent &event);
    void OnSingleEdit (wxCommandEvent &event);    

    // Project Menu
    void OnOpenProject(wxCommandEvent &event);
    void OnSaveProject(wxCommandEvent &event);
    void OnCloseProject(wxCommandEvent &event);
    
    // Inform menu
    void OnCompile (wxCommandEvent &event);
    void OnRunZcode(wxCommandEvent &event);
    void OnZcodeVersion(wxCommandEvent &event);
    
    // Glulxe Menu
    void OnCompileUlx(wxCommandEvent &event);
    void OnRunUlx(wxCommandEvent &event);        
    void OnCreateBlb(wxCommandEvent &event);    
    void OnCreateRes(wxCommandEvent &event);        
        
    // Metodi generici
    void OnClose (wxCloseEvent &event);
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);
    void OnNotebookPageChanged(wxAuiNotebookEvent& evt);
    void OnSelChanged(wxTreeEvent &event);
    
    // HELP MENU
    void OnViewDoc(wxCommandEvent &event);
    void OnTip(wxCommandEvent &event);    
   
 private:
     wxAuiManager m_mgr;

     wxAuiNotebook* auinotebook;
     wxTextCtrl* console;
     wxToolBar* toolbar;
     wxTreeCtrl* tree;

     
     // Impostazioni Object tree
     bool expandAllNodes;
     bool showObjects;     
     bool showGlobals;     
     bool showConstants;     
     bool showFunctions;     
     bool showClasses;     
     bool showIncludes;     
     bool showVerbs;          
     bool showLineNumber;          
     bool wrapMode;      
     bool showTips;    

     int untitled;
     int autoCompleteNumber;    // Number of char typed before window autocomplete     

     // INFORM STUFF
     wxString informCompiler;
     wxString libraryPath;     
     wxString zcodeversion;          
     wxString zcodeswitch;    
     
     wxString bres;         
     wxString blc;              
     wxString mainFile;                  

     // Funzioni di inzializzazione componenti
     wxTreeCtrl* CreateTreeCtrl();      // Definizione creazione albero
     wxMenuBar* CreateMenuBar();
     wxAuiNotebook* CreateNotebook();
     wxToolBar* CreateToolBarCtrl();
        
    
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
    EVT_MENU(MyFrame::ID_LoadFile,   MyFrame::OnLoadFile)
    EVT_MENU(MyFrame::ID_Exit,       MyFrame::OnExit)
    EVT_MENU(MyFrame::ID_About,      MyFrame::OnAbout)
    EVT_MENU(MyFrame::ID_Licence,    MyFrame::OnLicence)
    EVT_MENU(MyFrame::ID_Support,    MyFrame::OnSupport)    
    EVT_MENU(MyFrame::ID_NewFile,    MyFrame::OnNewFile)
    
    // MENU EDIT
    EVT_MENU (wxID_CUT,              MyFrame::OnSingleEdit)
    EVT_MENU (wxID_COPY,             MyFrame::OnSingleEdit)
    EVT_MENU (wxID_PASTE,            MyFrame::OnSingleEdit)
    
    // Search
    EVT_MENU(MyFrame::ID_GotoLine,     MyFrame::GotoLine)    
    EVT_MENU(MyFrame::ID_Find,         MyFrame::Search)
    EVT_MENU(MyFrame::ID_FindBack,     MyFrame::SearchBack)    //PL
    EVT_MENU(MyFrame::ID_NextMarker,   MyFrame::NextMarker)
    EVT_MENU(MyFrame::ID_ToggleMarker, MyFrame::ToggleMarker)    
    
    // Project Menu
    EVT_MENU (ID_OpenProject,            MyFrame::OnOpenProject)
    EVT_MENU (ID_SaveProject,            MyFrame::OnSaveProject)
    EVT_MENU (ID_CloseProject,           MyFrame::OnCloseProject)        
    
    // Inform    
    EVT_MENU (ID_Compile,            MyFrame::OnCompile)
    EVT_MENU (ID_RunZcode,           MyFrame::OnRunZcode)    
    EVT_MENU (ID_ZcodeVersion5,      MyFrame::OnZcodeVersion)    
    EVT_MENU (ID_ZcodeVersion8,      MyFrame::OnZcodeVersion)        
    
    // GLULXE MENU
    EVT_MENU (ID_CompileUlx,         MyFrame::OnCompileUlx)
    EVT_MENU (ID_RunUlx,             MyFrame::OnRunUlx)
    EVT_MENU (ID_CreateBlb,          MyFrame::OnCreateBlb)    
    EVT_MENU (ID_CreateRes,          MyFrame::OnCreateRes)        
    
        
    // Object Tree    
    EVT_MENU (ID_RefreshTree,        MyFrame::OnRefreshTree)    
    EVT_MENU (ID_ShowObject,         MyFrame::OnOptions)
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
//    EVT_MENU (myID_INDENTRED,        MyFrame::OnSingleEdit)    
    EVT_MENU (wxID_UNDO,             MyFrame::OnSingleEdit)
    EVT_MENU (wxID_REDO,             MyFrame::OnSingleEdit)
    EVT_MENU (ID_Save_File,          MyFrame::OnSaveFile)
    EVT_MENU (ID_Save_All,          MyFrame::OnSaveAll)      //PL

    // Opzioni
    EVT_MENU (myID_LINENUMBER,       MyFrame::OnEdit)
    EVT_MENU (myID_WRAPMODEON,       MyFrame::OnEdit)    
    EVT_MENU (ID_ShowTips,           MyFrame::OnOptions)    
    
    // Eventi sull'oggetto TREE
    EVT_TREE_SEL_CHANGED(wxID_ANY, MyFrame::OnSelChanged)
    
    // Eventi sull'oggetto notebook
    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MyFrame::OnNotebookPageClose)
    EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, MyFrame::OnNotebookPageChanged)
    
    // DOC MENU
    EVT_MENU (ID_Doc1,           MyFrame::OnViewDoc)        
    EVT_MENU (ID_Doc2,           MyFrame::OnViewDoc)        
    EVT_MENU (ID_Doc3,           MyFrame::OnViewDoc)                
    EVT_MENU (ID_Doc4,           MyFrame::OnViewDoc)                
    EVT_MENU (ID_Doc5,           MyFrame::OnViewDoc)     
    
    EVT_MENU (ID_Tip,           MyFrame::OnTip)                            
END_EVENT_TABLE()



// Funzioni di classe ---------------------------------------------------------
// MENU FILE ****************
void MyFrame::OnNewFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* fd = new wxFileDialog(this, "New File","","","*.*",
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
        Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER);
        stc->LoadFile (path);

        // Impostazioni del nuovo stc
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

        // Recupero tutte le wordlist dal file di configurazione
        pConfig->SetPath(_T("/AUTOCOMPLETIONLIST"));
        wxString wordlist = "";
        wxString str;
        long dummy;
        bool bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            wxString s = pConfig->Read(_T(str),_T(""));
            wordlist = wordlist + s + " ";
            bCont = pConfig->GetNextEntry(str, dummy);
        }
        pConfig->SetPath(_T("/"));

        stc->SetWordlist(wordlist);

        console->Clear();

        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
        auinotebook->AddPage(stc  , name, true , page_bmp);
        //auinotebook->SetSelection(auinotebook->GetSelection()+1);
        OnUpdateTree();
    }
}

// SHOW TIPS
void MyFrame::ShowTip()
{
    static size_t s_index = (size_t)-1;
    if ( s_index == (size_t)-1 )
    {
        srand(time(NULL));
        // this is completely bogus, we don't know how many lines are there
        // in the file, but who cares, it's a demo only...
        s_index = rand() % 5;
    }

    wxTipProvider *tipProvider = wxCreateFileTipProvider(_T("tips.txt"), s_index);
    bool showAtStartup = wxShowTip(this, tipProvider);
    s_index = tipProvider->GetCurrentTip();
    delete tipProvider;
    showTips = showAtStartup;
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
                                     _T(""),
                                     wxOK | wxCANCEL);
        
            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!="") {
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
                                     _T(""),
                                     wxOK | wxCANCEL);

            if (dialog.ShowModal() == wxID_OK) {
                selected = dialog.GetValue();
                position = e->GetCurrentPos();
            }
        }
        // Do the search
        if (selected!="") {
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






void MyFrame::SaveConfiguration() {
     pConfig->Write(_T("OBJECT_TREE_EXPAND_ALL_NODES"), expandAllNodes);
     pConfig->Write(_T("OBJECT_TREE_SHOW_OBJECTS"), showObjects);
     pConfig->Write(_T("OBJECT_TREE_SHOW_GLOBALS"), showGlobals);
     pConfig->Write(_T("OBJECT_TREE_SHOW_CONSTANTS"), showConstants);
     pConfig->Write(_T("OBJECT_TREE_SHOW_FUNCTIONS"), showFunctions);
     pConfig->Write(_T("OBJECT_TREE_SHOW_CLASSES"), showClasses);
     pConfig->Write(_T("OBJECT_TREE_SHOW_INCLUDES"), showIncludes);
     pConfig->Write(_T("OBJECT_TREE_SHOW_VERBS"), showVerbs);

     pConfig->Write(_T("SHOWLINENUMBERS"), showLineNumber);
     pConfig->Write(_T("WRAPMODE"), wrapMode);
     
     pConfig->Write(_T("SHOWTIPS"), showTips);
     
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
     showGlobals    = pConfig->Read(_T("OBJECT_TREE_SHOW_GLOBALS"), 1l) != 0;
     showConstants  = pConfig->Read(_T("OBJECT_TREE_SHOW_CONSTANTS"), 1l) != 0;
     showFunctions  = pConfig->Read(_T("OBJECT_TREE_SHOW_FUNCTIONS"), 1l) != 0;
     showClasses    = pConfig->Read(_T("OBJECT_TREE_SHOW_CLASSES"), 1l) != 0;
     showIncludes   = pConfig->Read(_T("OBJECT_TREE_SHOW_INCLUDES"), 1l) != 0;
     showVerbs      = pConfig->Read(_T("OBJECT_TREE_SHOW_VERBS"), 1l) != 0;

     showLineNumber = pConfig->Read(_T("SHOWLINENUMBERS"), 1l) != 0;
     wrapMode       = pConfig->Read(_T("WRAPMODE"), 1l) != 0;             
     
     showTips       = pConfig->Read(_T("SHOWTIPS"), 1l) != 0;             
     
     autoCompleteNumber = pConfig->Read(_T("AUTOCOMPLETE_NUMBER"), 1l)!=0;
     
     // Inform Stuff
     informCompiler = pConfig->Read("INFORMCOMPILER", _T(""));
     libraryPath="+include_path="+ 
                pConfig->Read("LIBRARYPATH1", _T(""))+_T(",")+
                pConfig->Read("LIBRARYPATH2", _T(""))+_T(",")+
                pConfig->Read("LIBRARYPATH3", _T(""));                
    bres = pConfig->Read("BRESPATH", _T(""));                
    blc  = pConfig->Read("BLCPATH", _T(""));                              
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
    for (int i=0; i<=pannelli ;i++) { 
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
        case ID_ShowGlobal: showGlobals=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowConstant: showConstants=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowFunction: showFunctions=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowClass: showClasses=event.IsChecked(); OnUpdateTree(); break;                        
        case ID_ShowInclude: showIncludes=event.IsChecked(); OnUpdateTree(); break;        
        case ID_ShowVerb: showVerbs=event.IsChecked(); OnUpdateTree(); break;                 
        case ID_ExpandAllAlways: expandAllNodes=event.IsChecked(); OnUpdateTree(); break;                                
        
        case ID_ShowTips: showTips=event.IsChecked(); break;                                                
    }
}

// MENU HELP
void MyFrame::OnTip(wxCommandEvent &event){
    ShowTip();    
}
// MENU DOC
void MyFrame::OnViewDoc(wxCommandEvent &event){
    int id = event.GetId();
//    wxMessageBox (_("Sono qui!"), _("Close abort"),  wxOK | wxICON_EXCLAMATION);
    wxString documento;
    switch (id)
    {
        case ID_Doc1: documento = pConfig->Read("PDF1_PATH", _T("")); break;
        case ID_Doc2: documento = pConfig->Read("PDF2_PATH", _T("")); break;
        case ID_Doc3: documento = pConfig->Read("PDF3_PATH", _T("")); break;
        case ID_Doc4: documento = pConfig->Read("PDF4_PATH", _T("")); break;
        case ID_Doc5: documento = pConfig->Read("PDF5_PATH", _T("")); break;
    }
    
    // Visualizzo il pdf o word   
    wxString comando = pConfig->Read("PDFREADER", _T(""))+" \""+documento+"\"";
    wxExecute(_T(comando));    
}


// MENU INFORM ******************************************************
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
    console->Clear();
    console->AppendText("Creating blb file...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString blcFile, blbFile;
    if (mainFile==""){
        blcFile = e->GetFilename();
        blbFile = e->GetFilename();
    }
    else{
        console->AppendText("Using MainFile: "+mainFile+"\n");
        blcFile = mainFile;
        blbFile = mainFile;
    }
    blcFile.Replace(".inf", ".blc", true);
    blbFile.Replace(".inf", ".blb", true);
    wxString comando =  "\""+blc +"\""+" "+"\""+blcFile+"\""+" "+"\""+blbFile+"\"";

    //wxString comando =  blc +" "+blcFile+" "+blbFile;    
    wxArrayString output;
    if ( wxExecute(_T(comando), output) != 0 )
    {
        console->AppendText(comando);
        console->AppendText("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }
        console->AppendText("OK.");
    }
    else
    {
        console->AppendText(comando);
        console->AppendText("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }
        console->AppendText("Ok.");
    }    
}

void MyFrame::OnCreateRes (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    console->Clear();
    console->AppendText("Creating Resources for BLB file...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString bresFile;
    if (mainFile==""){
        bresFile = e->GetFilename();
    }
    else{
        console->AppendText("Using MainFile: "+mainFile+"\n");
        bresFile = mainFile;
    }
    bresFile.Replace(".inf", "", true);
    wxString comando =  "\""+bres +"\""+" "+"\""+bresFile+"\"";
    wxArrayString output;
    if ( wxExecute(_T(comando), output) != 0 )
    {
        console->AppendText(comando);
        console->AppendText("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }

        console->AppendText("OK.");
    }
    else
    {
        console->AppendText(comando);
        console->AppendText("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }
        console->AppendText("Ok.");
    }
}


// Run Ulx
void MyFrame::OnRunUlx (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    console->Clear();
    console->AppendText("Running zcode...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome = e->GetFilename();
    nome.Replace(".inf", ".ulx", true);
    wxString comando =  pConfig->Read("GLULXINTERPRETER", _T("")) +" "+"\""+nome+"\"";
    console->AppendText(comando+"\n");
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
    console->Clear();
    console->AppendText("Compiling in ULX format...\n");
    wxArrayString output;

    // Recupero il nome del file da compilare
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome,ulx;
    if (mainFile==""){
        nome = e->GetFilename();
        ulx = e->GetFilename();
    }
    else{
        console->AppendText("Using MainFile: "+mainFile+"\n");
        nome = mainFile;
        ulx = mainFile;
    }
    ulx.Replace(".inf", ".ulx", true);
    wxString comando =  informCompiler +" -G "+libraryPath+" \""+nome+"\" "+"\""+ulx+"\"";

    //if ( wxExecute(_T("./inform.exe twocol.inf"), output) != 0 )
    if ( wxExecute(_T(comando), output) != 0 )
    {
        console->AppendText(comando);
        console->AppendText("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            //wxPrintf(_T("\t%s\n"), output[n].c_str());
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }

        console->AppendText("ERROR.");
    }
    else
    {
        console->AppendText(comando);
        console->AppendText("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            //wxPrintf(_T("\t%s\n"), output[n].c_str());
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }
        console->AppendText("Ok.");
    }
}

// Run zcode
void MyFrame::OnRunZcode (wxCommandEvent &event) {
    if (auinotebook->GetPageCount()==0){ return; }
    console->Clear();
    console->AppendText("Running zcode...\n");
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome = e->GetFilename();
    nome.Replace(".inf", zcodeversion, true);    
    wxString comando =  pConfig->Read("INFORMINTERPRETER", _T("")) +" "+"\""+nome+"\"";
    console->AppendText(comando+"\n");
    wxArrayString output; 
    wxExecute(_T(comando));
}

// Compile
void MyFrame::OnCompile (wxCommandEvent &event) {
    // Se non ho finetre aperte esco
    if (auinotebook->GetPageCount()==0){ return; }

    // Azzero la console
    console->Clear();
    console->AppendText("Compiling Z-Code...\n");
    wxArrayString output;
    
    // Recupero il nome del file da compilare
    Edit* e = (Edit*) auinotebook->GetPage(auinotebook->GetSelection());
    wxString nome,zcode;    
    if (mainFile==""){
        nome = e->GetFilename();
        zcode = e->GetFilename();
    }
    else{
        console->AppendText("Using MainFile: "+mainFile+"\n");        
        nome = mainFile;
        zcode = mainFile;        
    }
    zcode.Replace(".inf", zcodeversion, true);
    wxString comando =  informCompiler+" "+zcodeswitch+" "+libraryPath+" \""+nome+"\" "+" \""+zcode+"\" ";
    
    //if ( wxExecute(_T("./inform.exe twocol.inf"), output) != 0 )
    if ( wxExecute(_T(comando), output) != 0 )    
    {
        console->AppendText(comando);
        console->AppendText("\n");
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }
        console->AppendText("ERROR.");
    }
    else
    {
        console->AppendText(comando);
        console->AppendText("\n");        
        size_t count = output.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            console->AppendText(output[n].c_str());
            console->AppendText("\n");
        }
        console->AppendText("Ok.");
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
        //console->AppendText(wxString::Format("%d", item->GetPosition())+"\n");        
        //console->AppendText(wxString::Format("%d", item->GetPosition())
        //+" "+"start["+wxString::Format("%d", start)+"]"
        //+" "+"end["+wxString::Format("%d", end)+"]\n");
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
    console->Clear();     
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
    }    
    
    int pannelli = auinotebook->GetPageCount();
    for (int i=0; i<=pannelli ;i++){
        Edit* e = (Edit*) auinotebook->GetPage(i);
        if (e) {
            e->ProcessEvent (event);
        }
    }       
}
 
void MyFrame::LoadFile(wxString path, wxString name)
{
    Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER);
    stc->LoadFile (path);

    // Impostazioni del nuovo stc
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

    // Recupero tutte le wordlist per AUTOCOMPLETION dal file di configurazione
    pConfig->SetPath(_T("/AUTOCOMPLETIONLIST"));
    wxString wordlist = "";
    wxString str;
    long dummy;
    bool bCont = pConfig->GetFirstEntry(str, dummy);
    while(bCont){
        wxString s = pConfig->Read(_T(str),_T(""));
        wordlist = wordlist + s + " ";
        bCont = pConfig->GetNextEntry(str, dummy);
    }
    pConfig->SetPath(_T("/"));
    stc->SetWordlist(wordlist);
    stc->SetKeyWords (mySTC_TYPE_DEFAULT, wordlist);
    stc->SetAutoCompleteNumber(autoCompleteNumber);
    console->Clear();

    wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
    auinotebook->AddPage(stc  , name, true , page_bmp);
    OnUpdateTree();
}
 
void MyFrame::OnLoadFile(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* fd = new wxFileDialog(this, "Open File","","","*.*",
    wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,"filedlg");
    wxString path,name;
    if (fd->ShowModal() == wxID_OK ){
        path = fd->GetPath();
        name = fd->GetFilename();
        Edit* stc = new Edit( auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER);
        stc->LoadFile (path);
        
        // Impostazioni del nuovo stc
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

        // Recupero tutte le wordlist per AUTOCOMPLETION dal file di configurazione
        pConfig->SetPath(_T("/AUTOCOMPLETIONLIST"));
        wxString wordlist = "";
        wxString str;
        long dummy;
        bool bCont = pConfig->GetFirstEntry(str, dummy);
        while(bCont){
            wxString s = pConfig->Read(_T(str),_T(""));
            wordlist = wordlist + s + " ";
            bCont = pConfig->GetNextEntry(str, dummy);
        }
        pConfig->SetPath(_T("/"));
        stc->SetWordlist(wordlist);
        stc->SetKeyWords (mySTC_TYPE_DEFAULT, wordlist);
        stc->SetAutoCompleteNumber(autoCompleteNumber);
        console->Clear();
        
        wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
        auinotebook->AddPage(stc  , name, true , page_bmp);
        //auinotebook->SetSelection(auinotebook->GetSelection()+1);
        OnUpdateTree();        
    }   
} 

// MENU PROJECT
void MyFrame::OnOpenProject(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog* fd = new wxFileDialog(this, "Open Wide Project","","","*.wpf",
    wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,"filedlg");
    wxString path,name;
    if (fd->ShowModal() == wxID_OK ){
        path = fd->GetPath();
        name = fd->GetFilename();        
        // Per ora leggo solo la prima riga del file
        wxTextFile file(path);
        file.Open();
        wxString main_f = file.GetFirstLine();
        mainFile = path.Mid(0,path.Find('\\',true)+1) + main_f;
//        wxMessageBox ("Using main file ["+mainFile+"]", _("Main File"),  wxOK | wxICON_INFORMATION);
        console->Clear();
        console->AppendText("Using Main file ["+mainFile+"] ");
        
        // Apro il file
        LoadFile(mainFile,main_f);
    }
}

void MyFrame::OnSaveProject(wxCommandEvent& WXUNUSED(event))
{
}

void MyFrame::OnCloseProject(wxCommandEvent& WXUNUSED(event))
{
    if (mainFile!=""){
        wxMessageBox (mainFile, _("Remove Main File"),  wxOK | wxICON_INFORMATION);            
        mainFile="";
        console->Clear();
        console->AppendText("Main file removed.");        
    }
    else{
        wxMessageBox (_("No Project opened."),_("Warning"), wxOK | wxICON_WARNING);    
    }
}


// MENU ABOUT
void MyFrame::OnSupport(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;
    info.SetName(_(NOMEAPPLICAZIONE));
    info.SetVersion(_(VERSIONE));
    info.SetWebSite(_T("http://www.slade.altervista.org/"), _T("Home page"));
    info.SetDescription(wxString::FromAscii(
"SUPPORT WIDE\n"
"===============\n"
"\n"
"  If you like this program you can support it in these ways:\n"
"  \n"
"  1) Send a donation with PayPal to silver.slade@tiscali.it\n"
"  2) Offer your help for documentation/test/translation in your language\n"
"  3) Offer your help to make a linux/MacOs version of Wide (C++/WxWidgets knowledge required)\n"
    ));
    wxAboutBox(info);
}

void MyFrame::OnLicence(wxCommandEvent& WXUNUSED(event))
{

    wxAboutDialogInfo info;
    info.SetName(_(NOMEAPPLICAZIONE));
    info.SetVersion(_(VERSIONE));
    info.SetCopyright(_T("(C) 2007 - Alessandro Schillaci"));
    info.SetWebSite(_T("http://www.slade.altervista.org/"), _T("Home page"));
    info.SetDescription(wxString::FromAscii(
"GNU GPL LICENCE\n"
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
    info.SetCopyright(_T("(C) 2007 - silver.slade@tiscali.it"));
    info.AddDeveloper(_T("Alessandro Schillaci"));
    wxAboutBox(info);    
}
 
// Funzione che aggiunge un nodo alla root con i valori relativi
void MyFrame::OnUpdateTreeRegularExpression(wxString text, wxTreeItemId root, wxString nome, wxChar *pattern_global){
    int contatore=0;    
    wxRegEx re;
    if (!re.Compile(pattern_global, wxRE_ICASE|wxRE_NEWLINE) ){
        console->AppendText("Errore nella RE");
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
    wxString text = e->GetText();
    tree->DeleteAllItems();    
    wxArrayTreeItemIds items;
    wxTreeItemId root = tree->AddRoot(wxT("Object Tree"), 0);


    if (showObjects) OnUpdateTreeRegularExpression(text, root, "Object", "\n+[ \t\f\v]*Object[ \t\n\r\f\v]+(->[ \t\n\r\f\v]+)*([A-Za-z0-9_]+)");
    if (showGlobals) OnUpdateTreeRegularExpression(text, root, "Global", "\n+[ \t\f\v]*Global[ \t\n\r\f\v]+([A-Za-z0-9_]+)");
    if (showConstants) OnUpdateTreeRegularExpression(text, root, "Constant", "\n+[ \t\f\v]*Constant[ \t\n\r\f\v]+([A-Za-z0-9_]+)");
    if (showFunctions) OnUpdateTreeRegularExpression(text, root, "Function", "[;][ \t\n\r\f\v]*\\[[ \t\n\r\f\v]*([A-Za-z0-9_]+).*;");
    if (showClasses) OnUpdateTreeRegularExpression(text, root, "Class", "\n+[ \t\f\v]*Class[ \t\n\r\f\v]+([A-Za-z0-9_]+)");
    if (showIncludes) OnUpdateTreeRegularExpression(text, root, "Include", "\n+[ \t\f\v]*Include[ \t\f\v]+\"([A-Za-z0-9_]+)\"");
    if (showVerbs) OnUpdateTreeRegularExpression(text, root, "Verb", "\n+[ \t\f\v]*Verb[ \t\f\v]+\'([A-Za-z0-9_]+)'");

    if (expandAllNodes) {
        tree->ExpandAll();                
    }
    else{
        tree->Expand(root);     
    }
    
    // Scroll all'inizio dell'albero
    tree->ScrollTo(root);
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
   
                      
//   Edit* stc = new Edit( ctrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER);
//   stc->LoadFile (_T("principe.inf"));
//   ctrl->AddPage( stc , wxT("wxTextCtrl 2"));
//   ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some more text"),
//                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 2") );
                
   return ctrl;
}


// MENUBAR |--------------------------------------------------------------------
wxMenuBar* MyFrame::CreateMenuBar()
{
    wxMenuBar* mb = new wxMenuBar;

    // FILE MENU
    wxMenu* file = new wxMenu;    
    // FILE::NEW
    wxMenuItem *new_file = new wxMenuItem(file, ID_NewFile, _("&New file\tCtrl+N"));
    new_file->SetBitmap(new_xpm);
    file->Append(new_file);    
    
    // FILE::LOAD
    wxMenuItem *load_file = new wxMenuItem(file, ID_LoadFile, _("&Open file\tCtrl+O"));
    load_file->SetBitmap(fileopen_xpm);
    file->Append(load_file);

    // FILE::SAVE
    wxMenuItem *save_file = new wxMenuItem(file, ID_Save_File, _("&Save File\tCtrl+S"));
    save_file->SetBitmap(filesave_xpm);
    file->Append(save_file);
    
    //FILE::SAVEALL   //PL
    wxMenuItem *save_all = new wxMenuItem(file, ID_Save_All, _("&Save All\tCtrl+Shift+S"));
    save_all->SetBitmap(filesaveall_xpm);
    file->Append(save_all);
    
    file->AppendSeparator();
    
    wxMenuItem *quit = new wxMenuItem(file, ID_Exit, _("&Quit\tCtrl+Q"));
    quit->SetBitmap(quit_xpm);
    file->Append(quit);

    // EDIT MENU
    wxMenu* edit = new wxMenu;

    // EDIT::COPY
    wxMenuItem *copy = new wxMenuItem(edit, wxID_COPY, _("&Copy\tCtrl+C"));
    copy->SetBitmap(copy_xpm);
    edit->Append(copy);

    // EDIT::CUT
    wxMenuItem *cut = new wxMenuItem(edit, wxID_CUT, _("Cu&t\tCtrl+X"));
    cut->SetBitmap(cut_xpm);
    edit->Append(cut);
    
    // EDIT::PASTE
    wxMenuItem *paste = new wxMenuItem(edit, wxID_PASTE, _("&Paste\tCtrl+V"));
    paste->SetBitmap(paste_xpm);
    edit->Append(paste);    
    edit->AppendSeparator();

    edit->Append(wxID_UNDO, _("&Undo\tCtrl+Z"));
    edit->Append(wxID_REDO, _("&Redo\tCtrl+Y"));
    edit->AppendSeparator();
    
    // EDIT::INDENT
    wxMenuItem *indent_r = new wxMenuItem(edit, myID_INDENTINC, _("&Indent\tTab"));
    indent_r->SetBitmap(forward_xpm);
    edit->Append(indent_r);

    // EDIT::UNINDENT
//    wxMenuItem *indent_l = new wxMenuItem(edit, myID_INDENTRED, _("&Unindent\tShift+Tab"));
//    indent_l->SetBitmap(back_xpm);
//    edit->Append(indent_l);

    // SEARCH MENU
    wxMenu* search = new wxMenu;
    search->Append (ID_Find, _("&Find\tF3"));
    search->Append (ID_FindBack, _("&Find Backwards\tShift+F3"));   //PL
    search->Append (ID_FindReplace, _("Find and &replace\tCtrl+F3"));
    search->AppendSeparator();
    search->Append(ID_GotoLine, _("&Goto line\tCtrl+L"));
    search->AppendSeparator();
    search->Append(ID_NextMarker, _("&Next Marker\tF2"));
    
    // EDIT::INDENT
    wxMenuItem *marker = new wxMenuItem(search, ID_ToggleMarker, _("&Toggle Marker\tCtrl+F2"));
    marker->SetBitmap(addbookm_xpm);
    search->Append(marker);        
    //search->Append(ID_ToggleMarker, _("&Toggle Marker\tCtrl+F2")); 

    // PROJECT MENU
    wxMenu* project = new wxMenu;
    project->Append (ID_OpenProject, _("&Open Project\tCtrl+P"));
    project->Append (ID_SaveProject, _("&Save Project"));
    project->Append (ID_CloseProject, _("&Close Project"));
    
    
    // Inform MENU
    wxMenu* inform = new wxMenu;
    inform->Append (ID_Compile, _("&Compile Zcode\tF8"));
    inform->Append (ID_RunZcode, _("&Run Zcode\tCtrl+F8"));
    inform->AppendSeparator();
    inform->AppendRadioItem (ID_ZcodeVersion5, _("Zcode version 5"));
    inform->AppendRadioItem (ID_ZcodeVersion8, _("Zcode version 8"));
    
    
    // GLULXE MENU
    wxMenu* glulxe = new wxMenu;
    glulxe->Append (ID_CompileUlx, _("&Compile Ulx\tF9"));            
    glulxe->Append (ID_RunUlx, _("&Run Ulx\tCtrl+F9"));
    glulxe->AppendSeparator();
    glulxe->Append (ID_CreateRes, _("&Create Resources\tF10"));
    glulxe->Append (ID_CreateBlb, _("&Create Blb File\tF11"));
    glulxe->Append (wxID_ANY, _("&Run Blb\tCtrl+F11"));


    // OPTION MENU
    wxMenu* option = new wxMenu;
    option->AppendCheckItem (myID_LINENUMBER, _("Show line &numbers"));
    option->AppendCheckItem (myID_WRAPMODEON, _("&Wrap mode"));    
    option->AppendCheckItem (ID_ShowTips, _("&Show Tips at Startup"));    
    
    option->Check(myID_LINENUMBER, showLineNumber);
    option->Check(myID_WRAPMODEON, wrapMode);
    option->Check(ID_ShowTips, showTips);    

    // OBJECT TREE MENU
    wxMenu* otree = new wxMenu;

    otree->Append (ID_RefreshTree, _("Refresh Tree\tENTER"));
    otree->AppendCheckItem (ID_ExpandAllAlways, _("Expand always all nodes"));
    otree->AppendSeparator();
    otree->AppendCheckItem (ID_ShowObject, _("Show Objects"));
    otree->AppendCheckItem (ID_ShowGlobal, _("Show Globals"));
    otree->AppendCheckItem (ID_ShowConstant, _("Show Constants"));
    otree->AppendCheckItem (ID_ShowFunction, _("Show Functions"));
    otree->AppendCheckItem (ID_ShowClass, _("Show Classes"));
    otree->AppendCheckItem (ID_ShowInclude, _("Show Includes"));
    otree->AppendCheckItem (ID_ShowVerb, _("Show Verbs"));
    
    // Load configuration from file    
    otree->Check(ID_ExpandAllAlways, (pConfig->Read(_T("OBJECT_TREE_EXPAND_ALL_NODES"), 1l) != 0) );
    otree->Check(ID_ShowObject, (pConfig->Read(_T("OBJECT_TREE_SHOW_OBJECTS"), 1l) != 0) );
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


    wxMenu* help = new wxMenu;
    help->Append(ID_Licence, _("Licence"));    
    help->Append(ID_Support, _("Support"));        
    help->AppendSeparator();
    
    
    wxMenuItem *tips = new wxMenuItem(help, ID_Tip, _("Tip of the day"));
    tips->SetBitmap(tipicon_xpm);
    help->Append(tips);
  
    // HELP::ABOUT
    wxMenuItem *about = new wxMenuItem(help, ID_About, _("About Wide"));
    about->SetBitmap(help_xpm);
    help->Append(about);
    
    mb->Append(file, _("File"));
    mb->Append(edit, _("Edit"));
    mb->Append(search, _("Search"));        
    mb->Append(project , _("Project"));            
    mb->Append(inform, _("Inform"));
    mb->Append(glulxe, _("Glulxe"));    
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
//    wxBitmap tb2_bmp_fileopen = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16,16));
   
    tb2->AddTool(ID_LoadFile, fileopen_xpm,"Open File");
    tb2->AddTool(ID_NewFile, new_xpm,"New File");    
    tb2->AddTool(ID_Save_File, filesave_xpm, "Save File");
    tb2->AddTool(103, filesaveas_xpm, "Save File as...");
    tb2->AddTool(ID_Save_All, filesaveall_xpm, "Save All");
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
  
