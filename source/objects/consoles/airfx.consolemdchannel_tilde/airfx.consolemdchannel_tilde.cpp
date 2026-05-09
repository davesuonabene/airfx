#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/consolemdchannel.hpp"

using namespace c74::min;

class consolemdchannel_tilde : public airfx<consolemdchannel_tilde, airwindohhs::consolemdchannel::ConsoleMDChannel<double>>
{
    atom m_about_text = symbol{ airwindohhs::consolemdchannel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::consolemdchannel::k_name.data() };
    MIN_TAGS{ airwindohhs::consolemdchannel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    consolemdchannel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(consolemdchannel_tilde);
