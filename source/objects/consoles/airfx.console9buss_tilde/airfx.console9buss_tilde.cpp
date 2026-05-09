#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console9buss.hpp"

using namespace c74::min;

class console9buss_tilde : public airfx<console9buss_tilde, airwindohhs::console9buss::Console9Buss<double>>
{
    atom m_about_text = symbol{ airwindohhs::console9buss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console9buss::k_name.data() };
    MIN_TAGS{ airwindohhs::console9buss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console9buss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console9buss_tilde);
