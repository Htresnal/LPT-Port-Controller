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

#include "conio.h"
#include "stdlib.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>

#define wxUSE_DYNLIB_CLASS 1

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define LPT1 0x378 // for pin_init_user
#define LPT2 0x278 // for pin_init_user
#define LPTX1 0xD030 // for pin_init_user
#define LPTX2 0x3BC // for pin_init_user
#define LPTX 0x3BC // for pin_init_user

//#define IOCTL_READ_PORT_UCHAR	 CTL_CODE(40000, 0x801, 0, 0x00000000)
//#define IOCTL_WRITE_PORT_UCHAR	 CTL_CODE(40000, 0x802, 0, 0x00000000)
//#define CTL_CODE(t,f,m,a) (((t)<<16)|((a)<<14)|((f)<<2)|(m))

// Driver

int inst();
int start();

void *hdriver=NULL;
void *hdriver_inpout=NULL;
wchar_t path[MAX_PATH];
HINSTANCE hmodule;
SECURITY_ATTRIBUTES sa;
int sysver;

int Opendriver()
{
    inst();
    start();
	hdriver = CreateFile(L"\\\\.\\hwinterfacex64",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
    std::string myStr="[ERROR]:";
    myStr.append(SSTR(GetLastError()));
    wxMessageBox(myStr.c_str(), "Error", wxOK | wxOK_DEFAULT | wxICON_WARNING, 0);
    if(hdriver == INVALID_HANDLE_VALUE)
	{
        wxMessageBox("hwinterfacex64.sys could not be installed.", "Driver not found", wxOK | wxOK_DEFAULT | wxICON_WARNING, 0);
	}
	return 0;
}

int inst()
{
	wchar_t szDriverSys[MAX_PATH];
	wcscat(szDriverSys, L"hwinterfacex64.sys\0");

	SC_HANDLE Mgr;
	SC_HANDLE Ser;
	GetSystemDirectory(path , sizeof(path));
	wcscpy(path,L"System32\\Drivers\\hwinterfacex64.sys");
	std::wstring myWString(path);
	std::string myString(myWString.begin(),myWString.end());

	std::wifstream source("hwinterfacex64.sys", std::ios::binary);
    std::wofstream dest;
    dest.open(myString.c_str(),std::ios::binary);

    std::istreambuf_iterator<wchar_t> begin_source(source);
    std::istreambuf_iterator<wchar_t> end_source;
    std::ostreambuf_iterator<wchar_t> begin_dest(dest);
    std::copy(begin_source, end_source, begin_dest);

    source.close();
    dest.close();

	Mgr = OpenSCManager (NULL, NULL,SC_MANAGER_ALL_ACCESS);
	if (Mgr == NULL)
	{							//No permission to create service
		if (GetLastError() == ERROR_ACCESS_DENIED)
		{
			return 5;  // error access denied
		}
	}
	else
	{
		wchar_t szFullPath[MAX_PATH] = L"System32\\Drivers\\\0";
		wcscat(szFullPath, szDriverSys);
		Ser = CreateService (Mgr,
			L"hwinterfacex64",
			L"hwinterfacex64",
			SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
			SERVICE_SYSTEM_START,
			SERVICE_ERROR_NORMAL,
			szFullPath,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
			);
	}
	CloseServiceHandle(Ser);
	CloseServiceHandle(Mgr);

	return 0;
}

int start()
{
	SC_HANDLE  Mgr;
	SC_HANDLE  Ser;

	Mgr = OpenSCManager (NULL, NULL,SC_MANAGER_ALL_ACCESS);

	if (Mgr == NULL)
	{							//No permission to create service
		if (GetLastError() == ERROR_ACCESS_DENIED)
		{
			Mgr = OpenSCManager (NULL, NULL,GENERIC_READ);
			Ser = OpenService(Mgr,L"hwinterfacex64",GENERIC_EXECUTE);
			if (Ser)
			{    // we have permission to start the service
				if(!StartService(Ser,0,NULL))
				{
					CloseServiceHandle (Ser);
					return 4; // we could open the service but unable to start
				}
			}
		}
	}
	else
	{// Successfuly opened Service Manager with full access
		Ser = OpenService(Mgr,L"hwinterfacex64",GENERIC_EXECUTE);
		if (Ser)
		{
			if(!StartService(Ser,0,NULL))
			{
				CloseServiceHandle (Ser);
				wxMessageBox("Full access, but cannot start.", "ERROR", wxOK | wxOK_DEFAULT | wxICON_WARNING, 0);
				return 3; // opened the Service handle with full access permission, but unable to start
			}
			else
			{
				CloseServiceHandle (Ser);
				wxMessageBox("Handle closed.", "ERROR", wxOK | wxOK_DEFAULT | wxICON_WARNING, 0);
				return 0;
			}
		}
	}
	return 1;
}

void Closedriver(void)
{
	if (hdriver)
	{
		CloseHandle(hdriver);
		hdriver=NULL;
	}
}


int LPTPorts_input;
int LPTPorts_output;

DWORD BytesReturned;
    unsigned char Buffer[3];
    unsigned short *pBuffer=(unsigned short *)&Buffer;

//(((40000)<<16)|((0x00000000)<<14)|((0x801)<<2)|(0))
short LTPControllerFrame::Inp32(short portAddress)
{
    *pBuffer = LOWORD(portAddress);
    Buffer[2] = 0;
    DeviceIoControl(hdriver_inpout,
        -1673519100,
        &Buffer,
        2,
        &Buffer,
        1,
        &BytesReturned,
        NULL);
return((int)Buffer[0]);
}

//(((40000)<<16)|((0x00000000)<<14)|((0x802)<<2)|(0))
void LTPControllerFrame::Out32(short portAddress, short pinData)
{
    *pBuffer = LOWORD(portAddress);
    Buffer[2] = LOBYTE(pinData);
    DeviceIoControl(hdriver_inpout,
        -1673519096,
        &Buffer,
        3,
        NULL,
        0,
        &BytesReturned,
        NULL);
return;
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

class LPTPort
{
public:
    LPTPort(int, int, int, int, int, void *);
    bool currState;
    int type;
    bool inverted;
    int direction;
    int pinCode;
    void *button;
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
const long LTPControllerFrame::ID_BITMAPBUTTONX = wxNewId();
const long LTPControllerFrame::ID_BUTTON2 = wxNewId();
const long LTPControllerFrame::ID_PANEL1 = wxNewId();
const long LTPControllerFrame::idMenuQuit = wxNewId();
const long LTPControllerFrame::idMenuAbout = wxNewId();
const long LTPControllerFrame::ID_STATUSBAR1 = wxNewId();
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
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(500,500));
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(216,240), wxSize(400,256), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBitmap1 = new wxStaticBitmap(Panel1, ID_STATICBITMAP1, wxBitmap(wxImage(_T("D:\\cprojects\\LTPController\\res\\LPTPortImg.png")).Rescale(wxSize(460,109).GetWidth(),wxSize(460,109).GetHeight())), wxPoint(16,184), wxSize(460,109), wxTRANSPARENT_WINDOW, _T("ID_STATICBITMAP1"));
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Connect"), wxPoint(408,304), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BitmapButtonX = new wxBitmapButton(Panel1, ID_BITMAPBUTTONX, wxNullBitmap, wxPoint(136,304), wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER|wxTRANSPARENT_WINDOW, wxDefaultValidator, _T("ID_BITMAPBUTTONX"));
    BitmapButtonX->Hide();
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("Test(0)"), wxPoint(312,304), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnButton1Click);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LTPControllerFrame::OnButton2Click);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&LTPControllerFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&LTPControllerFrame::OnAbout);
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
    LPTPorts_input=0;
    LPTPorts_output=0;
    for (int i=1;i<18;i++)
    {
        if (((LPTPort *)LPTPort_Pins[i])->direction==portDir_in || ((LPTPort *)LPTPort_Pins[i])->direction==portDir_both)
        {
            LPTPorts_input=LPTPorts_input+((LPTPort *)LPTPort_Pins[i])->pinCode;
        }
        if (((LPTPort *)LPTPort_Pins[i])->direction==portDir_out || ((LPTPort *)LPTPort_Pins[i])->direction==portDir_both)
        {
            LPTPorts_output=LPTPorts_output+((LPTPort *)LPTPort_Pins[i])->pinCode;
        }
    }
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
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void LTPControllerFrame::clear_pin(int pins)
{
	Out32(LPTX, Inp32(LPTX)&((pins&0xFF)^0xFF));
    wxMessageBox(wxString::Format(wxT("%i"),Inp32(LPTX+0x2)), _("LPTX"));
	//dll_devOut32(LPTX+0x2, (dll_devInp32(LPTX+0x2)|((pins>>8)&0xF)&0xB)&(((pins>>8)&0xF)&(0xB^0x1FFFF))^0x1FFFF);
	Out32(LPTX+0x2, (Inp32(LPTX+0x2)|((pins>>8)&0xF)&0xB)&(((pins>>8)&0xF)&(0xB^0x1FFFF))^0x1FFFF);
return;
}

void LTPControllerFrame::invert_pin(int pins)
{
	Out32(LPTX, Inp32(LPTX) ^ pins & 0xFF);
	wxMessageBox(wxString::Format(wxT("%i"),Inp32(LPTX+0x2)), _("LPTX"));

	//dll_devOut32(LPTX+0x2, dll_devInp32( LPTX+0x2 ) ^ ((pins >> 8) & 0xF));
	Out32(LPTX+0x2, Inp32(LPTX+0x2) ^ (pins>>8)&0xFF);
return;
}

int LTPControllerFrame::pin_is_set(int pins)
{
return ((((Inp32(LPTX+0x1)>>3)&(pins>>12)&0x1F)^0x10) << 12) |
        (Inp32(LPTX) ^ 0xFF) |
        (((Inp32(LPTX+0x2) ^ (0xB ^ 0x1FFFF)) & 0xF) << 8);
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
    int myPins=Inp32( LPTX );
    for (int i=1;i<26;i++)
    {
        if ((myPins&((LPTPort *)LPTPort_Pins[i])->pinCode)!=0)
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
    int myPins=Inp32( LPTX );
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
    Out32(LPTX,0x0);
    Out32(LPTX+0x2,0x0);
    Refresh_state();
	LED_refreshState();
	for (int i=1;i<26;i++)
    {
        ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->Hide();
        ((wxBitmapButton *)((LPTPort *)LPTPort_Pins[i])->button)->Show();
    }
    Opendriver();
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

void LTPControllerFrame::OnButton2Click(wxCommandEvent& event)
{
    wxMessageBox(wxString::Format(wxT("%i"),Inp32(LPTX)), _("LPTX"));
}
