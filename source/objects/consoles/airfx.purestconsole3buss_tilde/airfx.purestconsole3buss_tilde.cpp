#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/purestconsole3buss.hpp"

using namespace c74::min;

class purestconsole3buss_tilde : public airfx<purestconsole3buss_tilde, airwindohhs::purestconsole3buss::PurestConsole3Buss<double>>
{
    atom m_about_text = symbol{ airwindohhs::purestconsole3buss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::purestconsole3buss::k_name.data() };
    MIN_TAGS{ airwindohhs::purestconsole3buss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    purestconsole3buss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(purestconsole3buss_tilde);
