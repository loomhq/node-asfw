
#include "node_asfw.h"
#include <string>
#include <sstream>

#if defined(WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

NAN_METHOD(_SetForegroundWindow) {
    if (info[0]->IsUndefined()) {
        Nan::ThrowError("SetForegroundWindow needs one argument");
    }
    auto maybeArg = Nan::To<int64_t>(info[0]);
    if (maybeArg.IsNothing()) {
        Nan::ThrowError("SetForegroundWindow needs a number argument");
    }

    HWND hwnd = (HWND)(maybeArg.FromJust());
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);

    if (GetWindowPlacement(hwnd, &wp)) {
        if (wp.showCmd == SW_MINIMIZE || wp.showCmd == SW_SHOWMINIMIZED || wp.showCmd == SW_SHOWMINNOACTIVE) {
            ShowWindow(hwnd, SW_RESTORE);
        }
        else {
            ShowWindow(hwnd, SW_SHOW);
        }
    }
    else {
        ShowWindow(hwnd, SW_SHOW);
    }

    BOOL ret = SetForegroundWindow(hwnd);
    if (ret == 0) {
        DWORD lastError = GetLastError();
        if (lastError != 0) {
            std::stringstream ss;
            ss << "Windows error code " << lastError;
            auto err = ss.str();
            Nan::ThrowError(err.c_str());
        }
    }
}

#else // defined(WIN32)

NAN_METHOD(_SetForegroundWindow) {
    Nan::ThrowError("SetForegroundWindow is not implemented on this platform");
}

#endif // !defined(WIN32)

