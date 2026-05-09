#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/consolemcchannel.hpp"

using namespace c74::min;

class consolemcchannel_tilde : public airfx<consolemcchannel_tilde, airwindohhs::consolemcchannel::ConsoleMCChannel<double>>
{
    atom m_about_text = symbol{ airwindohhs::consolemcchannel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::consolemcchannel::k_name.data() };
    MIN_TAGS{ airwindohhs::consolemcchannel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    consolemcchannel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(consolemcchannel_tilde);
