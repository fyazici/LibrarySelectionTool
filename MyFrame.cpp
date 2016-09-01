//
// Created by fatih on 9/1/16.
//

#include "MyFrame.h"
#include <fstream>
#include <boost/algorithm/string.hpp>

MyFrame::MyFrame()
        : wxFrame(nullptr, -1, wxT("Title"), wxDefaultPosition, wxSize(600, 600)) {
    // Directory control boxes
//  {
//      {
    // Source directory control box
    wxBoxSizer *mboxDirLeft = new wxBoxSizer(wxHORIZONTAL);

    mTxtSrcDir = new wxStaticText(this, TXT_SRCDIR, wxT("Kaynak: "));
    mBtnSrcDir = new wxButton(this, BTN_SRCDIR, wxT("Aç..."));

    mboxDirLeft->Add(mTxtSrcDir, 1, wxEXPAND | wxALL, 5);
    mboxDirLeft->Add(mBtnSrcDir, 0, wxALL, 5);
//      }

//      {
    //right control box
    wxBoxSizer *mboxTopRight = new wxBoxSizer(wxHORIZONTAL);

//      }
//  }

    // List boxes
//  {
    wxBoxSizer *mboxMid = new wxBoxSizer(wxHORIZONTAL);

    mLeftPane = new wxListBox(this, LBOX_LEFT);
    //mLeftPane->SetMaxSize(wxSize(700, 1000));

    // Mid buttons
//      {
    wxBoxSizer *mboxListCtlButtons = new wxBoxSizer(wxVERTICAL);
    mBtnMoveAllRight = new wxButton(this, BTN_MOVEALLRIGHT, wxT("Hepsini\nEkle >>"));
    mBtnMoveRight = new wxButton(this, BTN_MOVERIGHT, wxT("Ekle >"));
    mBtnMoveLeft = new wxButton(this, BTN_MOVELEFT, wxT("< Çıkar"));
    mBtnMoveAllLeft = new wxButton(this, BTN_MOVEALLRIGHT, wxT("<< Hepsini\nÇıkar"));
    mboxListCtlButtons->Add(mBtnMoveAllRight, 0, wxEXPAND | wxALL, 5);
    mboxListCtlButtons->Add(mBtnMoveRight, 0, wxEXPAND | wxALL, 5);
    mboxListCtlButtons->Add(mBtnMoveLeft, 0, wxEXPAND | wxALL, 5);
    mboxListCtlButtons->Add(mBtnMoveAllLeft, 0, wxEXPAND | wxALL, 5);
//      }

    mRightPane = new wxListBox(this, LBOX_RIGHT);
    //mRightPane->SetMaxSize(wxSize(700, 1000));

    mboxMid->Add(mLeftPane, 10, wxEXPAND | wxALL, 5);
    mboxMid->Add(mboxListCtlButtons, 1, wxALIGN_CENTER_VERTICAL, 5);
    mboxMid->Add(mRightPane, 10, wxEXPAND | wxALL, 5);
//  }

    wxBoxSizer *mboxTop = new wxBoxSizer(wxHORIZONTAL);
    mboxTop->Add(mboxDirLeft, 1, wxEXPAND | wxALL, 5);
    mboxTop->Add(mboxTopRight, 1, wxEXPAND | wxALL, 5);

    wxBoxSizer *mboxBottom = new wxBoxSizer(wxHORIZONTAL);
    mTxtTotalSize = new wxStaticText(this, TXT_TOTSIZE, wxT("Toplam boyut: "));
    mBtnFilterExt = new wxButton(this, BTN_FILTER, wxT("Filtre"));
    mBtnSaveList = new wxButton(this, BTN_SAVE, wxT("Listeyi kaydet..."));

    mboxBottom->Add(mTxtTotalSize, 100, wxEXPAND | wxALL, 5);
    mboxBottom->Add(mBtnFilterExt, 1, wxALL, 5);
    mboxBottom->Add(mBtnSaveList, 1, wxALL, 5);

    wxBoxSizer *mboxRoot = new wxBoxSizer(wxVERTICAL);

    mboxRoot->Add(mboxTop, 0, wxEXPAND | wxALL, 5);
    mboxRoot->Add(mboxMid, 1, wxEXPAND | wxALL, 5);
    mboxRoot->Add(mboxBottom, 0, wxEXPAND | wxALL, 5);

    mBtnSrcDir->Bind(wxEVT_BUTTON, &MyFrame::OnSrcButtonClick, this);

    mBtnMoveAllRight->Bind(wxEVT_BUTTON, &MyFrame::OnMoveAllRightButtonClick, this);
    mBtnMoveRight->Bind(wxEVT_BUTTON, &MyFrame::OnMoveRightButtonClick, this);
    mBtnMoveLeft->Bind(wxEVT_BUTTON, &MyFrame::OnMoveLeftButtonClick, this);
    mBtnMoveAllLeft->Bind(wxEVT_BUTTON, &MyFrame::OnMoveAllLeftButtonClick, this);

    mLeftPane->Bind(wxEVT_LISTBOX_DCLICK, &MyFrame::OnLeftListDoubleClick, this);
    mRightPane->Bind(wxEVT_LISTBOX_DCLICK, &MyFrame::OnRightListDoubleClick, this);

    mBtnFilterExt->Bind(wxEVT_BUTTON, &MyFrame::OnFilterExtButtonClick, this);
    mBtnSaveList->Bind(wxEVT_BUTTON, &MyFrame::OnSaveListButtonClick, this);

    this->SetSizer(mboxRoot);
    this->Layout();

    this->Centre(wxBOTH);
}

void MyFrame::OnSrcButtonClick(wxCommandEvent &) {
    wxDirDialog dlg(this, wxT("Kaynak klasör"));
    if (dlg.ShowModal() == wxID_OK) {
        setSourceDirectory(dlg.GetPath());
    }
}

void MyFrame::OnDstButtonClick(wxCommandEvent &) {
    wxDirDialog dlg(this, wxT("Hedef klasör"));
    if (dlg.ShowModal() == wxID_OK) {
        setDestDirectory(dlg.GetPath());
    }
}

void MyFrame::OnMoveRightButtonClick(wxCommandEvent &e) {
    int sel = mLeftPane->GetSelection();
    if (sel != -1) {
        moveSourceToDest(sel);
    }
    mLeftPane->SetSelection(std::min(sel, (int)m_sourceFiles.size()-1));
    mLeftPane->SetFocus();
}

void MyFrame::OnMoveLeftButtonClick(wxCommandEvent &) {
    int sel = mRightPane->GetSelection();
    if (sel != -1) {
        moveDestToSource(sel);
    }
    mRightPane->SetSelection(std::min(sel, (int)m_destFiles.size()-1));
    mRightPane->SetFocus();
}

void MyFrame::OnMoveAllRightButtonClick(wxCommandEvent &) {
    moveAllSourceToDest();
}

void MyFrame::OnMoveAllLeftButtonClick(wxCommandEvent &) {
    moveAllDestToSource();
}


void MyFrame::OnRightListDoubleClick(wxCommandEvent &) {
    int sel = mRightPane->GetSelection();
    if (sel != -1) {
        playDestItem(sel);
    }
}

void MyFrame::OnLeftListDoubleClick(wxCommandEvent &) {
    int sel = mLeftPane->GetSelection();
    if (sel != -1) {
        playSourceItem(sel);
    }
}

void MyFrame::OnFilterExtButtonClick(wxCommandEvent &) {
    const std::string ext = ".mp3";

    std::vector<file_info> new_src;

    for (const file_info& x : m_sourceFiles) {
        std::string s = x.get_ext();
        boost::algorithm::to_lower(s);
        if (ext == s)
            new_src.emplace_back(x);
    }

    std::swap(m_sourceFiles, new_src);
    updateListBoxes();
}


void MyFrame::OnSaveListButtonClick(wxCommandEvent &) {
    wxFileDialog dlg(this, wxT("Kaydedilecek dosya adı"), wxEmptyString, wxEmptyString, wxT("*.txt"), wxFD_SAVE);
    if (dlg.ShowModal() == wxID_OK) {
        std::ofstream os(dlg.GetPath().mb_str(wxConvUTF8));
        os << "-*- coding:utf8 -*-\n";
        os << "List of selected files (ordered alphabetically): \n";
        for (const auto& x : m_destFiles)
            os << x.get_name() << '\n';
        os << "-- END --\n";
    }
}

void MyFrame::setSourceDirectory(const wxString& path) {
    auto path2 = std::string(path.mb_str(wxConvUTF8).data());

    m_sourceDir = path2;
    m_sourceFiles = file_info::from_directory(path2);

    std::sort(m_sourceFiles.begin(), m_sourceFiles.end(),
              [] (const file_info& a, const file_info& b) {
                  return a.get_name() < b.get_name();
              });

    updateDirTexts();
    updateListBoxes();
}

void MyFrame::setDestDirectory(const wxString& path) {
    auto path2 = std::string(path.mb_str(wxConvUTF8).data());
    m_destDir = path2;
    updateDirTexts();
}

void MyFrame::moveSourceToDest(int sel) {
    auto item = m_sourceFiles[sel];
    m_sourceFiles.erase(m_sourceFiles.begin() + sel);
    m_destFiles.push_back(item);
    std::sort(m_destFiles.begin(), m_destFiles.end());
    updateListBoxes();
}

void MyFrame::moveDestToSource(int sel) {
    auto item = m_destFiles[sel];
    m_destFiles.erase(m_destFiles.begin() + sel);
    m_sourceFiles.push_back(item);
    std::sort(m_sourceFiles.begin(), m_sourceFiles.end());
    updateListBoxes();
}

void MyFrame::playSourceItem(int sel) {
    std::cout << "left dbl" << std::endl;
}

void MyFrame::playDestItem(int sel) {
    std::cout << "right dbl" << std::endl;
}

template<typename LB, typename T>
void fill_pane(const LB& lb, std::vector<T> v) {
    std::vector<wxString> v2;
    v2.resize(v.size());
    std::transform(v.begin(), v.end(), v2.begin(), [](const file_info& a) { return wxString(a.get_name().c_str(), wxConvUTF8); });
    lb->Clear();
    lb->Append(v2);
};

void MyFrame::updateListBoxes() {
    fill_pane(mLeftPane, m_sourceFiles);
    fill_pane(mRightPane, m_destFiles);

    size_t total = std::accumulate(m_destFiles.begin(), m_destFiles.end(), (size_t) 0,
        [](size_t a, const file_info& b) { return a + b.get_size(file_info::size_format::mbytes); });

    mTxtTotalSize->SetLabelText(wxString("Toplam boyut: " + std::to_string(total) + " MB"));
}

void MyFrame::updateDirTexts() {
    mTxtSrcDir->SetLabelText(m_sourceDir);
}

void MyFrame::moveAllSourceToDest() {
    m_destFiles.insert(m_destFiles.end(), m_sourceFiles.begin(), m_sourceFiles.end());
    m_sourceFiles.clear();
    updateListBoxes();
}

void MyFrame::moveAllDestToSource() {
    m_sourceFiles.insert(m_sourceFiles.end(), m_destFiles.begin(), m_destFiles.end());
    m_destFiles.clear();
    updateListBoxes();
}
