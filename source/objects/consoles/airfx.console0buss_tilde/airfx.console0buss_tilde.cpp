#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console0buss.hpp"

using namespace c74::min;

class console0buss_tilde : public airfx<console0buss_tilde, airwindohhs::console0buss::Console0Buss<double>>
{
    atom m_about_text = symbol{ airwindohhs::console0buss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console0buss::k_name.data() };
    MIN_TAGS{ airwindohhs::console0buss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console0buss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console0buss_tilde);
