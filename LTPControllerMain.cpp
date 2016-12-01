/***************************************************************
 * Name:      LTPControllerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2016-06-18
 * Copyright:  ()
 * License:
 **************************************************************/
#include "LTPControllerMain.h"
#include <wx/msgdlg.h>
#include <wx/dynlib.h>

#define wxUSE_DYNLIB_CLASS 1

HINSTANCE hmodule;

typedef void	(__stdcall *lpOut32)(short, short);
typedef short	(__stdcall *lpInp32)(short);
typedef BOOL	(__stdcall *lpIsInpOutDriverOpen)(void);
typedef BOOL	(__stdcall *lpIsXP64Bit)(void);

lpOut32 gfpOut32;
lpInp32 gfpInp32;

int Opendriver()
{
    hmodule=LoadLibraryA ( "InpOutx64.DLL" );

    gfpOut32 = (lpOut32)GetProcAddress(hmodule, "Out32");
    gfpInp32 = (lpInp32)GetProcAddress(hmodule, "Inp32");

    if (!(lpIsInpOutDriverOpen)GetProcAddress(hmodule, "IsInpOutDriverOpen"))
    {
        wxMessageBox("Problems during program initialization.", "Unable to start the driver", wxOK | wxOK_DEFAULT | wxICON_WARNING, 0);
    }
return 0;
}

void Closedriver(void)
{
	if (hmodule)
	{
	    FreeLibrary(hmodule);
	}
}

wxString B_GreenOn="D:\\cprojects\\LTPController\\res\\GreenOn.png";
wxString B_GreenOff="D:\\cprojects\\LTPController\\res\\GreenOff.png";
wxString B_WhiteOn="D:\\cprojects\\LTPController\\res\\WhiteOn.png";
wxString B_WhiteOff="D:\\cprojects\\LTPController\\res\\WhiteOff.png";
wxString B_RedOn="D:\\cprojects\\LTPController\\res\\RedOn.png";
wxString B_RedOff="D:\\cprojects\\LTPController\\res\\RedOff.png";
wxString B_GrayOff="D:\\cprojects\\LTPController\\res\\GrayOff.png";

enum port_direction
{
    portDir_out,
    portDir_in,
    portDir_both,
    portDir_ground
};

enum port_type
{
    portType_data,
    portType_status,
    portType_control
};

LPTPort::LPTPort(int in_currState, int in_type, int in_inverted, int in_direction, int in_pinCode, void *in_button)
{
    currState=in_currState;
    type=in_type;
    inverted=in_inverted;
    direction=in_direction;
    pinCode=in_pinCode;
    button=in_button;
}

//(*InternalHeaders(LTPControllerFrame)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(LTPControllerFrame)
const long LTPControllerFrame::ID_STATICBITMAP1 = wxNewId();
const long LTPControllerFrame::ID_BUTTON1 = wxNewId();
const long LTPControllerFrame::ID_COMBOBOX1 = wxNewId();
const long LTPControllerFrame::ID_STATICTEXT1 = wxNewId();
const long LTPControllerFrame::ID_PANEL1 = wxNewId();
//*)
// BITMAP BUTTON
const long LTPControllerFrame::ID_BITMAPBUTTON1 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON2 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON3 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON4 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON5 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON6 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON7 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON8 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON9 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON10 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON11 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON12 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON13 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON14 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON15 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON16 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON17 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON18 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON19 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON20 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON21 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON22 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON23 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON24 = wxNewId();
const long LTPControllerFrame::ID_BITMAPBUTTON25 = wxNewId();

BEGIN_EVENT_TABLE(LTPControllerFrame,wxFrame)
    //(*EventTable(LTPControllerFrame)
    //*)
END_EVENT_TABLE()

LTPControllerFrame::LTPControllerFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(LTPControllerFrame)
    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(500,210));
    SetMinSize(wxSize(516,248));
    SetMaxSize(wxSize(516,248));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(216,240), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBitmap1 = new wxStaticBitmap(Panel1, ID_STATICBITMAP1, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\LPTPortImg.png")).Rescale(wxSize(460,109).GetWidth(),wxSize(460,109).GetHeight())), wxPoint(16,56), wxSize(460,109), wxTRANSPARENT_WINDOW, _T("ID_STATICBITMAP1"));
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Connect"), wxPoint(400,176), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    ComboBox1 = new wxComboBox(Panel1, ID_COMBOBOX1, wxEmptyString, wxPoint(384,32), wxSize(82,21), 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComboBox1->SetSelection( ComboBox1->Append(_("0xD030")) );
    ComboBox1->Append(_("0x378"));
    ComboBox1->Append(_("0x278"));
    ComboBox1->Append(_("0x3BC"));
    ComboBox1->Append(_("0xD020"));
    ComboBox1->Append(_("0xB000"));
    ComboBox1->Append(_("0xB400"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Port address:"), wxPoint(384,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnButton1Click);
    //*)

    BitmapButton1 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON1, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(360,31), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
    BitmapButton2 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON2, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(337,31), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
    BitmapButton3 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON3, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(313,31), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
    BitmapButton4 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON4, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(289,31), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON4"));
    BitmapButton5 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON5, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(265,31), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON5"));
    BitmapButton6 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON6, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(241,31), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON6"));
    BitmapButton7 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON7, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(217,31), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON7"));
    BitmapButton8 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON8, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(193,32), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON8"));
    BitmapButton9 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON9, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(169,32), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON9"));
    BitmapButton10 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON10, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(145,32), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON10"));
    BitmapButton11 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON11, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(121,32), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON11"));
    BitmapButton12 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON12, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(98,32), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON12"));
    BitmapButton13 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON13, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(74,32), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON13"));
    BitmapButton14 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON14, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(349,55), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON14"));
    BitmapButton15 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON15, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(325,55), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON15"));
    BitmapButton16 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON16, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(301,55), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON16"));
    BitmapButton17 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON17, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(277,55), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON17"));
    BitmapButton18 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON18, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(253,55), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON18"));
    BitmapButton19 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON19, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(229,55), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON19"));
    BitmapButton20 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON20, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(205,56), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON20"));
    BitmapButton21 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON21, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(181,56), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON21"));
    BitmapButton22 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON22, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(157,56), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON22"));
    BitmapButton23 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON23, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(133,56), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON23"));
    BitmapButton24 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON24, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(109,56), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON24"));
    BitmapButton25 = new wxBitmapButton(StaticBitmap1, ID_BITMAPBUTTON25, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\GrayOff.png"))), wxPoint(85,56), wxSize(19,19), wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTON25"));
    Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton1Click);
    Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton2Click);
    Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton3Click);
    Connect(ID_BITMAPBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton4Click);
    Connect(ID_BITMAPBUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton5Click);
    Connect(ID_BITMAPBUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton6Click);
    Connect(ID_BITMAPBUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton7Click);
    Connect(ID_BITMAPBUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton8Click);
    Connect(ID_BITMAPBUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton9Click);
    Connect(ID_BITMAPBUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton10Click);
    Connect(ID_BITMAPBUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton11Click);
    Connect(ID_BITMAPBUTTON12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton12Click);
    Connect(ID_BITMAPBUTTON13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton13Click);
    Connect(ID_BITMAPBUTTON14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton14Click);
    Connect(ID_BITMAPBUTTON15,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton15Click);
    Connect(ID_BITMAPBUTTON16,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton16Click);
    Connect(ID_BITMAPBUTTON17,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton17Click);
    Connect(ID_BITMAPBUTTON18,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton18Click);
    Connect(ID_BITMAPBUTTON19,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton19Click);
    Connect(ID_BITMAPBUTTON20,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton20Click);
    Connect(ID_BITMAPBUTTON21,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton21Click);
    Connect(ID_BITMAPBUTTON22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton22Click);
    Connect(ID_BITMAPBUTTON23,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton23Click);
    Connect(ID_BITMAPBUTTON24,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton24Click);
    Connect(ID_BITMAPBUTTON25,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnBitmapButton25Click);

    this->BitmapButton1->Disable();
    this->BitmapButton2->Disable();
    this->BitmapButton3->Disable();
    this->BitmapButton4->Disable();
    this->BitmapButton5->Disable();
    this->BitmapButton6->Disable();
    this->BitmapButton7->Disable();
    this->BitmapButton8->Disable();
    this->BitmapButton9->Disable();
    this->BitmapButton10->Disable();
    this->BitmapButton11->Disable();
    this->BitmapButton12->Disable();
    this->BitmapButton13->Disable();
    this->BitmapButton14->Disable();
    this->BitmapButton15->Disable();
    this->BitmapButton16->Disable();
    this->BitmapButton17->Disable();
    this->BitmapButton18->Disable();
    this->BitmapButton19->Disable();
    this->BitmapButton20->Disable();
    this->BitmapButton21->Disable();
    this->BitmapButton22->Disable();
    this->BitmapButton23->Disable();
    this->BitmapButton24->Disable();
    this->BitmapButton25->Disable();

    LPTPort_Pins.resize(26);
    LPTPort_Pins[0]=new LPTPort(false,portType_status,false,portDir_ground,0x0,BitmapButton1);

    LPTPort_Pins[1]=new LPTPort(false,portType_control,true,portDir_both,0x100,BitmapButton1);
    LPTPort_Pins[2]=new LPTPort(false,portType_data,false,portDir_out,0x1,BitmapButton2);
    LPTPort_Pins[3]=new LPTPort(false,portType_data,false,portDir_out,0x2,BitmapButton3);
    LPTPort_Pins[4]=new LPTPort(false,portType_data,false,portDir_out,0x4,BitmapButton4);
    LPTPort_Pins[5]=new LPTPort(false,portType_data,false,portDir_out,0x8,BitmapButton5);
    LPTPort_Pins[6]=new LPTPort(false,portType_data,false,portDir_out,0x10,BitmapButton6);
    LPTPort_Pins[7]=new LPTPort(false,portType_data,false,portDir_out,0x20,BitmapButton7);
    LPTPort_Pins[8]=new LPTPort(false,portType_data,false,portDir_out,0x40,BitmapButton8);
    LPTPort_Pins[9]=new LPTPort(false,portType_data,false,portDir_out,0x80,BitmapButton9);
    LPTPort_Pins[10]=new LPTPort(false,portType_status,false,portDir_in,0x8000,BitmapButton10);
    LPTPort_Pins[11]=new LPTPort(false,portType_status,true,portDir_in,0x10000,BitmapButton11);
    LPTPort_Pins[12]=new LPTPort(false,portType_status,false,portDir_in,0x4000,BitmapButton12);
    LPTPort_Pins[13]=new LPTPort(false,portType_status,false,portDir_in,0x2000,BitmapButton13);
    LPTPort_Pins[14]=new LPTPort(false,portType_control,true,portDir_both,0x200,BitmapButton14);
    LPTPort_Pins[15]=new LPTPort(false,portType_status,false,portDir_in,0x1000,BitmapButton15);
    LPTPort_Pins[16]=new LPTPort(false,portType_control,false,portDir_both,0x400,BitmapButton16);
    LPTPort_Pins[17]=new LPTPort(false,portType_control,true,portDir_both,0x800,BitmapButton17);

    LPTPort_Pins[18]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton18);
    LPTPort_Pins[19]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton19);
    LPTPort_Pins[20]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton20);
    LPTPort_Pins[21]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton21);
    LPTPort_Pins[22]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton22);
    LPTPort_Pins[23]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton23);
    LPTPort_Pins[24]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton24);
    LPTPort_Pins[25]=new LPTPort(false,portType_control,false,portDir_ground,0x0,BitmapButton25);

    LPTX=0x0;
}

LTPControllerFrame::~LTPControllerFrame()
{
    //(*Destroy(LTPControllerFrame)
    //*)
    Closedriver();
}

void LTPControllerFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void LTPControllerFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_("This application was made to test out the LPT control abilities of the InpOut32 library."), _("About"));
}

void LTPControllerFrame::clear_pin(int pins)
{
	gfpOut32(LPTX, gfpInp32(LPTX)&((pins&0xFF)^0xFF));
	gfpOut32(LPTX+0x2, (gfpInp32(LPTX+0x2)|((pins>>8)&0xF)&0xB)&(((pins>>8)&0xF)&(0xB^0x1FFFF))^0x1FFFF);
return;
}

void LTPControllerFrame::invert_pin(int pins)
{
	gfpOut32(LPTX, gfpInp32(LPTX) ^ pins & 0xFF);
	//gfpOut32(LPTX+0x2, gfpInp32(LPTX+0x2) ^ (pins>>8)&0xFF);
	gfpOut32(LPTX+0x2, gfpInp32(LPTX+0x2) ^ (pins>>8)&0xFF);
return;
}

int LTPControllerFrame::pin_is_set(int pins)
{
return ((((gfpInp32(LPTX+0x1)>>3)&(pins>>12)&0x1F)^0x10) << 12) | (gfpInp32(LPTX) ^ 0xFF) | (((gfpInp32(LPTX+0x2) ^ (0xB ^ 0x1FFFF)) & 0xF) << 8);
}

void LTPControllerFrame::LED_refreshState()
{
    for (int i=1;i<26;i++)
    {
        if (((LPTPort *)LPTPort_Pins[i])->direction==portDir_both)
        {
            if (((LPTPort *)LPTPort_Pins[i])->currState)
            {
                wxImage newImage(wxTRANSLATE(B_WhiteOn),wxBITMAP_TYPE_PNG);
                ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->SetBitmap(newImage);
            }
            else
            {
                wxImage newImage(wxTRANSLATE(B_WhiteOff),wxBITMAP_TYPE_PNG);
                ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->SetBitmap(newImage);
            }
        }
        else if (((LPTPort *)LPTPort_Pins[i])->direction==portDir_out)
        {
            if (((LPTPort *)LPTPort_Pins[i])->currState)
            {
                wxImage newImage(wxTRANSLATE(B_GreenOn),wxBITMAP_TYPE_PNG);
                ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->SetBitmap(newImage);
            }
            else
            {
                wxImage newImage(wxTRANSLATE(B_GreenOff),wxBITMAP_TYPE_PNG);
                ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->SetBitmap(newImage);
            }
        }
        else if (((LPTPort *)LPTPort_Pins[i])->direction==portDir_in)
        {
            if (((LPTPort *)LPTPort_Pins[i])->currState)
            {
                wxImage newImage(wxTRANSLATE(B_RedOn),wxBITMAP_TYPE_PNG);
                ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->SetBitmap(newImage);
            }
            else
            {
                wxImage newImage(wxTRANSLATE(B_RedOff),wxBITMAP_TYPE_PNG);
                ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->SetBitmap(newImage);
            }
        }
        else if (((LPTPort *)LPTPort_Pins[i])->direction==portDir_ground)
        {
            wxImage newImage(wxTRANSLATE(B_GrayOff),wxBITMAP_TYPE_PNG);
            ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->SetBitmap(newImage);
        }
    }
return;
}

void LTPControllerFrame::LED_refreshState(int refreshPins)
{
    if (((LPTPort *)LPTPort_Pins[refreshPins])->direction==portDir_both)
    {
        if (((LPTPort *)LPTPort_Pins[refreshPins])->currState)
        {
            wxImage newImage(wxTRANSLATE(B_WhiteOn),wxBITMAP_TYPE_PNG);
            ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[refreshPins])->button)->SetBitmap(newImage);
        }
        else
        {
            wxImage newImage(wxTRANSLATE(B_WhiteOff),wxBITMAP_TYPE_PNG);
            ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[refreshPins])->button)->SetBitmap(newImage);
        }
    }
    else if (((LPTPort *)LPTPort_Pins[refreshPins])->direction==portDir_out)
    {
        if (((LPTPort *)LPTPort_Pins[refreshPins])->currState)
        {
            wxImage newImage(wxTRANSLATE(B_GreenOn),wxBITMAP_TYPE_PNG);
            ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[refreshPins])->button)->SetBitmap(newImage);
        }
        else
        {
            wxImage newImage(wxTRANSLATE(B_GreenOff),wxBITMAP_TYPE_PNG);
            ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[refreshPins])->button)->SetBitmap(newImage);
        }
    }
    else if (((LPTPort *)LPTPort_Pins[refreshPins])->direction==portDir_in)
    {
        if (((LPTPort *)LPTPort_Pins[refreshPins])->currState)
        {
            wxImage newImage(wxTRANSLATE(B_RedOn),wxBITMAP_TYPE_PNG);
            ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[refreshPins])->button)->SetBitmap(newImage);
        }
        else
        {
            wxImage newImage(wxTRANSLATE(B_RedOff),wxBITMAP_TYPE_PNG);
            ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[refreshPins])->button)->SetBitmap(newImage);
        }
    }
    else if (((LPTPort *)LPTPort_Pins[refreshPins])->direction==portDir_ground)
    {
        wxImage newImage(wxTRANSLATE(B_GrayOff),wxBITMAP_TYPE_PNG);
        ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[refreshPins])->button)->SetBitmap(newImage);
    }
return;
}

void LTPControllerFrame::Refresh_state()
{
    int tmpPorts;
    for (int i=1;i<26;i++)
    {
        if (((LPTPort *)LPTPort_Pins[i])->type==portType_data)
        {
            tmpPorts=gfpInp32( LPTX );
        }
        else if (((LPTPort *)LPTPort_Pins[i])->direction==portType_control)
        {
            tmpPorts=gfpInp32( LPTX+0x2 );
        }

        if ((tmpPorts&((LPTPort *)LPTPort_Pins[i])->pinCode)!=0)
        {
            ((LPTPort *)LPTPort_Pins[i])->currState=1;
        }
        else
        {
            ((LPTPort *)LPTPort_Pins[i])->currState=0;
        }
    }
return;
}

void LTPControllerFrame::Refresh_state(int refreshPins)
{
    int myPins;

    if (((LPTPort *)LPTPort_Pins[refreshPins])->type==portType_data)
    {
        myPins=gfpInp32( LPTX );
    }
    else if (((LPTPort *)LPTPort_Pins[refreshPins])->direction==portType_control)
    {
        myPins=gfpInp32( LPTX+0x2 );
    }

    if ((myPins&((LPTPort *)LPTPort_Pins[refreshPins])->pinCode)!=0)
    {
        ((LPTPort *)LPTPort_Pins[refreshPins])->currState=1;
    }
    else
    {
        ((LPTPort *)LPTPort_Pins[refreshPins])->currState=0;
    }
return;
}

void LTPControllerFrame::OnButton1Click(wxCommandEvent& event)
{
    Opendriver();
    LPTX=strtol(ComboBox1->GetValue(), NULL, 16);
    //gfpOut32(LPTX,0x0);
    //gfpOut32(LPTX+0x2,0x0);
    Refresh_state();
	LED_refreshState();

    this->BitmapButton1->Enable();
    this->BitmapButton2->Enable();
    this->BitmapButton3->Enable();
    this->BitmapButton4->Enable();
    this->BitmapButton5->Enable();
    this->BitmapButton6->Enable();
    this->BitmapButton7->Enable();
    this->BitmapButton8->Enable();
    this->BitmapButton9->Enable();
    this->BitmapButton10->Enable();
    this->BitmapButton11->Enable();
    this->BitmapButton12->Enable();
    this->BitmapButton13->Enable();
    this->BitmapButton14->Enable();
    this->BitmapButton15->Enable();
    this->BitmapButton16->Enable();
    this->BitmapButton17->Enable();
    this->BitmapButton18->Enable();
    this->BitmapButton19->Enable();
    this->BitmapButton20->Enable();
    this->BitmapButton21->Enable();
    this->BitmapButton22->Enable();
    this->BitmapButton23->Enable();
    this->BitmapButton24->Enable();
    this->BitmapButton25->Enable();
}

void LTPControllerFrame::UI_LEDPanel_button_click(unsigned myButton)
{
    if (((LPTPort *)LPTPort_Pins[myButton])->direction==portDir_both || ((LPTPort *)LPTPort_Pins[myButton])->direction==portDir_out)
    {
        invert_pin(((LPTPort *)LPTPort_Pins[myButton])->pinCode);
        Refresh_state(myButton);
        LED_refreshState(myButton);
    }
}

void LTPControllerFrame::OnBitmapButton1Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(1);
}

void LTPControllerFrame::OnBitmapButton2Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(2);
}

void LTPControllerFrame::OnBitmapButton3Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(3);
}

void LTPControllerFrame::OnBitmapButton4Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(4);
}

void LTPControllerFrame::OnBitmapButton5Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(5);
}

void LTPControllerFrame::OnBitmapButton6Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(6);
}

void LTPControllerFrame::OnBitmapButton7Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(7);
}

void LTPControllerFrame::OnBitmapButton8Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(8);
}

void LTPControllerFrame::OnBitmapButton9Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(9);
}

void LTPControllerFrame::OnBitmapButton10Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(10);
}

void LTPControllerFrame::OnBitmapButton11Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(11);
}

void LTPControllerFrame::OnBitmapButton12Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(12);
}

void LTPControllerFrame::OnBitmapButton13Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(13);
}

void LTPControllerFrame::OnBitmapButton14Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(14);
}

void LTPControllerFrame::OnBitmapButton15Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(15);
}

void LTPControllerFrame::OnBitmapButton16Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(16);
}

void LTPControllerFrame::OnBitmapButton17Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(17);
}

void LTPControllerFrame::OnBitmapButton18Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(18);
}

void LTPControllerFrame::OnBitmapButton19Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(19);
}

void LTPControllerFrame::OnBitmapButton20Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(20);
}

void LTPControllerFrame::OnBitmapButton21Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(21);
}

void LTPControllerFrame::OnBitmapButton22Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(22);
}

void LTPControllerFrame::OnBitmapButton23Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(23);
}

void LTPControllerFrame::OnBitmapButton24Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(24);
}

void LTPControllerFrame::OnBitmapButton25Click(wxCommandEvent& event)
{
    UI_LEDPanel_button_click(25);
}
