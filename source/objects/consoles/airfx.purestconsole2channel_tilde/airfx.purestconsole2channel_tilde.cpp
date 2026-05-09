#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/purestconsole2channel.hpp"

using namespace c74::min;

class purestconsole2channel_tilde : public airfx<purestconsole2channel_tilde, airwindohhs::purestconsole2channel::PurestConsole2Channel<double>>
{
    atom m_about_text = symbol{ airwindohhs::purestconsole2channel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::purestconsole2channel::k_name.data() };
    MIN_TAGS{ airwindohhs::purestconsole2channel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    purestconsole2channel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(purestconsole2channel_tilde);
