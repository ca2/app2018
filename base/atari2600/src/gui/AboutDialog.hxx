//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2015 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: AboutDialog.hxx 3182 2015-07-10 18:59:03Z stephena $
//============================================================================

#ifndef ABOUT_DIALOG_HXX
#define ABOUT_DIALOG_HXX

class OSystem;
class DialogContainer;
class CommandSender;
class ButtonWidget;
class StaticTextWidget;


class AboutDialog : public Dialog
{
  public:
    AboutDialog(OSystem& osystem, DialogContainer& parent,
                const GUI::Font& font);
    virtual ~AboutDialog();

  private:
    void handleCommand(CommandSender* sender, int cmd, int data, int id) override;
    void updateStrings(int page, int lines, string& title);
    void displayInfo();

    void loadConfig() override { displayInfo(); }

  private:
    ButtonWidget* myNextButton;
    ButtonWidget* myPrevButton;

    StaticTextWidget* myTitle;
    vector<StaticTextWidget*> myDesc;
    vector<string> myDescStr;

    int myPage;
    int myNumPages;
    int myLinesPerPage;

  private:
    // Following constructors and assignment operators not supported
    AboutDialog() = delete;
    AboutDialog(const AboutDialog&) = delete;
    AboutDialog(AboutDialog&&) = delete;
    AboutDialog& operator=(const AboutDialog&) = delete;
    AboutDialog& operator=(AboutDialog&&) = delete;
};

#endif
