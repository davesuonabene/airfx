#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/everyconsole.hpp"

using namespace c74::min;

class everyconsole_tilde : public airfx<everyconsole_tilde, airwindohhs::everyconsole::EveryConsole<double>>
{
    atom m_about_text = symbol{ airwindohhs::everyconsole::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::everyconsole::k_name.data() };
    MIN_TAGS{ airwindohhs::everyconsole::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    everyconsole_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(everyconsole_tilde);
