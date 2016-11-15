/***************************************************************
 * Name:      LTPControllerApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2016-06-18
 * Copyright:  ()
 * License:
 **************************************************************/

#include "LTPControllerApp.h"

//(*AppHeaders
#include "LTPControllerMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(LTPControllerApp);

bool LTPControllerApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	LTPControllerFrame* Frame = new LTPControllerFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
