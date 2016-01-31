#include "twitter/utility.h"
#include <cpprest/asyncrt_utils.h>

namespace twitter {
bool open_browser(const std::string &uri) {
#if defined(_WIN32) && !defined(__cplusplus_winrt)
    // NOTE: Windows desktop only.
    auto r = ShellExecuteA(NULL, "open",
                           utility::conversions::utf16_to_utf8(uri).c_str(),
                           NULL, NULL, SW_SHOWNORMAL);

    return true;
#elif defined(__APPLE__)
    // NOTE: OS X only.
    std::string browser_cmd(U("open \"") + uri + U("\""));
    system(browser_cmd.c_str());

    return true;
#else
    // NOTE: Linux/X11 only.
    std::string browser_cmd(U("xdg-open \"") + uri + U("\""));
    system(browser_cmd.c_str());

    return true;
#endif
}
}
