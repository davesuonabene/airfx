#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console5buss.hpp"

using namespace c74::min;

class console5buss_tilde : public airfx<console5buss_tilde, airwindohhs::console5buss::Console5Buss<double>>
{
    atom m_about_text = symbol{ airwindohhs::console5buss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console5buss::k_name.data() };
    MIN_TAGS{ airwindohhs::console5buss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console5buss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console5buss_tilde);
