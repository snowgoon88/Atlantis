/***************************************************************
 * Name:      EditorApp.cpp
 * Purpose:   Code for Application Class
 * Author:     (snowgoon88@gmail.com)
 * Created:   2016-09-25
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "EditorApp.h"
#include "EditorMain.h"

IMPLEMENT_APP(EditorApp);

bool EditorApp::OnInit()
{
    EditorFrame* frame = new EditorFrame(0L, _("wxWidgets Application Template"));
    
    frame->Show();
    
    return true;
}
