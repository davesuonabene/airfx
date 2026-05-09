#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console5darkch.hpp"

using namespace c74::min;

class console5darkch_tilde : public airfx<console5darkch_tilde, airwindohhs::console5darkch::Console5DarkCh<double>>
{
    atom m_about_text = symbol{ airwindohhs::console5darkch::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console5darkch::k_name.data() };
    MIN_TAGS{ airwindohhs::console5darkch::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console5darkch_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console5darkch_tilde);
