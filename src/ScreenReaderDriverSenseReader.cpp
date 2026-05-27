#include "ScreenReaderDriverSenseReader.h"

ScreenReaderDriverSenseReader::ScreenReaderDriverSenseReader() :
ScreenReaderDriver(L"SenseReader", true, true),
pinterface(NULL),
hwnd(NULL)
{
  Connect(FindWindow(L"XVSRD", NULL));
}

ScreenReaderDriverSenseReader::~ScreenReaderDriverSenseReader()
{
  Disconnect();
}

void ScreenReaderDriverSenseReader::Connect(HWND target)
{
  Disconnect();
  if (!target) return;
  CLSID clsid_sensereader = __uuidof(SenseReaderApplication);
  IID iid_ixvapplication = __uuidof(IXVApplication);
  if (FAILED(CoCreateInstance(clsid_sensereader, NULL, CLSCTX_LOCAL_SERVER, iid_ixvapplication, (void **)&pinterface))) {
    pinterface = NULL;
    return;
  }
  hwnd = target;
}

void ScreenReaderDriverSenseReader::Disconnect()
{
  if (pinterface) {
    try { pinterface->Release(); } catch (...) {}
    pinterface = NULL;
  }
  hwnd = NULL;
}

bool ScreenReaderDriverSenseReader::Speak(const wchar_t *str, bool interrupt)
{
  if (!IsActive()) return false;
  if (interrupt && !Silence()) return false;
  try {
    pinterface->Speak(const_cast<BSTR>(str));
    return true;
  } catch (...) {
    Disconnect();
    return false;
  }
}

bool ScreenReaderDriverSenseReader::Braille(const wchar_t *str)
{
  return false;
}

bool ScreenReaderDriverSenseReader::Silence()
{
  if (!pinterface) return false;
  try {
    pinterface->StopSpeaking();
    return true;
  } catch (...) {
    Disconnect();
    return false;
  }
}

bool ScreenReaderDriverSenseReader::IsActive()
{
  HWND current = FindWindow(L"XVSRD", NULL);
  if (!current) {
    Disconnect();
    return false;
  }
  if (current != hwnd || !pinterface) {
    Connect(current);
  }
  return pinterface != NULL;
}

bool ScreenReaderDriverSenseReader::Output(const wchar_t *str, bool interrupt)
{
  return ScreenReaderDriverSenseReader::Speak(str, interrupt);
}
