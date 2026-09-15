#include "c74_min.h"
#include "airfx.hpp"
#include "lo-fi/tapebias.hpp"

using namespace c74::min;

class tapebias_tilde : public airfx<tapebias_tilde, airwindohhs::tapebias::TapeBias<double>>
{
    atom m_about_text = symbol{ airwindohhs::tapebias::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::tapebias::k_name.data() };
    MIN_TAGS{ airwindohhs::tapebias::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    tapebias_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(tapebias_tilde);
