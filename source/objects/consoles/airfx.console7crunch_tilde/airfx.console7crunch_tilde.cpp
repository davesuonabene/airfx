#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console7crunch.hpp"

using namespace c74::min;

class console7crunch_tilde : public airfx<console7crunch_tilde, airwindohhs::console7crunch::Console7Crunch<double>>
{
    atom m_about_text = symbol{ airwindohhs::console7crunch::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console7crunch::k_name.data() };
    MIN_TAGS{ airwindohhs::console7crunch::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console7crunch_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console7crunch_tilde);
