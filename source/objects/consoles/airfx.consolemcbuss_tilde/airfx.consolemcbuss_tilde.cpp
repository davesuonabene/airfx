#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/consolemcbuss.hpp"

using namespace c74::min;

class consolemcbuss_tilde : public airfx<consolemcbuss_tilde, airwindohhs::consolemcbuss::ConsoleMCBuss<double>>
{
    atom m_about_text = symbol{ airwindohhs::consolemcbuss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::consolemcbuss::k_name.data() };
    MIN_TAGS{ airwindohhs::consolemcbuss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    consolemcbuss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(consolemcbuss_tilde);
