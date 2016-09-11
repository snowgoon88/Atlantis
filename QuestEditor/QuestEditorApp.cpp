/***************************************************************
 * Name:      QuestEditorApp.cpp
 * Purpose:   Code for Application Class
 * Author:     (snowgoon88@gmail.com)
 * Created:   2016-09-07
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "QuestEditorApp.h"
#include "QuestEditorMain.h"

// Construct App and provide main entry point
IMPLEMENT_APP(QuestEditorApp);

bool QuestEditorApp::OnInit()
{
    QuestEditorFrame* frame = new QuestEditorFrame( (wxFrame*) NULL, _("wxWidgets Application Template"), wxSize(1700,600));

    frame->Show();

    return true;
}
