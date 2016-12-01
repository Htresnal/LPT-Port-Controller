/***************************************************************
 * Name:      LTPControllerMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2016-06-18
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef LTPCONTROLLERMAIN_H
#define LTPCONTROLLERMAIN_H

//(*Headers(LTPControllerFrame)
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/combobox.h>
//*)
#include <wx/bmpbuttn.h>
#include <vector>
#include <windows.h>
#include "conio.h"
#include <cstdlib>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "inpout32.h"

class LPTPort;

class LTPControllerFrame: public wxFrame
{
    public:
        std::vector<void *> LPTPort_Pins;
        void clear_pin(int pins);
        void invert_pin(int pins);
        int pin_is_set(int pins);
        void UI_LEDPanel_button_click(unsigned );
        void Refresh_state();
        void Refresh_state(int );
        void LED_refreshState();
        void LED_refreshState(int );
        LTPControllerFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~LTPControllerFrame();
        int LPTX;

    private:

        //(*Handlers(LTPControllerFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnButton2Click(wxCommandEvent& event);
        void OnComboBox1Selected(wxCommandEvent& event);
        //*)
        // BITMAP BUTTON
        void OnBitmapButton1Click(wxCommandEvent& event);
        void OnBitmapButton2Click(wxCommandEvent& event);
        void OnBitmapButton3Click(wxCommandEvent& event);
        void OnBitmapButton4Click(wxCommandEvent& event);
        void OnBitmapButton5Click(wxCommandEvent& event);
        void OnBitmapButton6Click(wxCommandEvent& event);
        void OnBitmapButton7Click(wxCommandEvent& event);
        void OnBitmapButton8Click(wxCommandEvent& event);
        void OnBitmapButton9Click(wxCommandEvent& event);
        void OnBitmapButton10Click(wxCommandEvent& event);
        void OnBitmapButton11Click(wxCommandEvent& event);
        void OnBitmapButton12Click(wxCommandEvent& event);
        void OnBitmapButton13Click(wxCommandEvent& event);
        void OnBitmapButton14Click(wxCommandEvent& event);
        void OnBitmapButton15Click(wxCommandEvent& event);
        void OnBitmapButton16Click(wxCommandEvent& event);
        void OnBitmapButton17Click(wxCommandEvent& event);
        void OnBitmapButton18Click(wxCommandEvent& event);
        void OnBitmapButton19Click(wxCommandEvent& event);
        void OnBitmapButton20Click(wxCommandEvent& event);
        void OnBitmapButton21Click(wxCommandEvent& event);
        void OnBitmapButton22Click(wxCommandEvent& event);
        void OnBitmapButton23Click(wxCommandEvent& event);
        void OnBitmapButton24Click(wxCommandEvent& event);
        void OnBitmapButton25Click(wxCommandEvent& event);

        //(*Identifiers(LTPControllerFrame)
        static const long ID_STATICBITMAP1;
        static const long ID_BUTTON1;
        static const long ID_COMBOBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_PANEL1;
        //*)
        // BITMAP BUTTON
        static const long ID_BITMAPBUTTON1;
        static const long ID_BITMAPBUTTON2;
        static const long ID_BITMAPBUTTON3;
        static const long ID_BITMAPBUTTON4;
        static const long ID_BITMAPBUTTON5;
        static const long ID_BITMAPBUTTON6;
        static const long ID_BITMAPBUTTON7;
        static const long ID_BITMAPBUTTON8;
        static const long ID_BITMAPBUTTON9;
        static const long ID_BITMAPBUTTON10;
        static const long ID_BITMAPBUTTON11;
        static const long ID_BITMAPBUTTON12;
        static const long ID_BITMAPBUTTON13;
        static const long ID_BITMAPBUTTON14;
        static const long ID_BITMAPBUTTON15;
        static const long ID_BITMAPBUTTON16;
        static const long ID_BITMAPBUTTON17;
        static const long ID_BITMAPBUTTON18;
        static const long ID_BITMAPBUTTON19;
        static const long ID_BITMAPBUTTON20;
        static const long ID_BITMAPBUTTON21;
        static const long ID_BITMAPBUTTON22;
        static const long ID_BITMAPBUTTON23;
        static const long ID_BITMAPBUTTON24;
        static const long ID_BITMAPBUTTON25;

        //(*Declarations(LTPControllerFrame)
        wxButton* Button1;
        wxStaticBitmap* StaticBitmap1;
        wxPanel* Panel1;
        wxStaticText* StaticText1;
        wxComboBox* ComboBox1;
        //*)
        // BITMAP BUTTON
        wxBitmapButton* BitmapButton1;
        wxBitmapButton* BitmapButton2;
        wxBitmapButton* BitmapButton3;
        wxBitmapButton* BitmapButton4;
        wxBitmapButton* BitmapButton5;
        wxBitmapButton* BitmapButton6;
        wxBitmapButton* BitmapButton7;
        wxBitmapButton* BitmapButton8;
        wxBitmapButton* BitmapButton9;
        wxBitmapButton* BitmapButton10;
        wxBitmapButton* BitmapButton11;
        wxBitmapButton* BitmapButton12;
        wxBitmapButton* BitmapButton13;
        wxBitmapButton* BitmapButton14;
        wxBitmapButton* BitmapButton15;
        wxBitmapButton* BitmapButton16;
        wxBitmapButton* BitmapButton17;
        wxBitmapButton* BitmapButton18;
        wxBitmapButton* BitmapButton19;
        wxBitmapButton* BitmapButton20;
        wxBitmapButton* BitmapButton21;
        wxBitmapButton* BitmapButton22;
        wxBitmapButton* BitmapButton23;
        wxBitmapButton* BitmapButton24;
        wxBitmapButton* BitmapButton25;

        DECLARE_EVENT_TABLE()
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

#endif // LTPCONTROLLERMAIN_H
