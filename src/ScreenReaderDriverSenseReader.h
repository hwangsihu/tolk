#import "C:\\program files\\xvision\\sense reader\\xvsrd.exe"
#ifndef _SCREEN_READER_DRIVER_SENSEREADER_H_
#define _SCREEN_READER_DRIVER_SENSEREADER_H_
#include <windows.h>
#include "ScreenReaderDriver.h"
using namespace SenseReader;

class ScreenReaderDriverSenseReader : public ScreenReaderDriver
{
public:
  ScreenReaderDriverSenseReader();
  ~ScreenReaderDriverSenseReader();
  bool Speak(const wchar_t *str, bool interrupt);
  bool Braille(const wchar_t *str);
  bool IsSpeaking() { return false; }
  bool Silence();
  bool IsActive();
  bool Output(const wchar_t *str, bool interrupt);

private:
  void Connect(HWND target);
  void Disconnect();

  IXVApplication *pinterface;
  HWND hwnd;
};
#endif
