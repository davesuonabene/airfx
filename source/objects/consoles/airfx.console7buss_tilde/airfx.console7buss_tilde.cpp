#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console7buss.hpp"

using namespace c74::min;

class console7buss_tilde : public airfx<console7buss_tilde, airwindohhs::console7buss::Console7Buss<double>>
{
    atom m_about_text = symbol{ airwindohhs::console7buss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console7buss::k_name.data() };
    MIN_TAGS{ airwindohhs::console7buss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console7buss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console7buss_tilde);
