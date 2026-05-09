#include "c74_min.h"
#include "airfx.hpp"
#include "lo-fi/derez3.hpp"

using namespace c74::min;

class derez3_tilde : public airfx<derez3_tilde, airwindohhs::derez3::DeRez3<double>>
{
    atom m_about_text = symbol{ airwindohhs::derez3::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::derez3::k_name.data() };
    MIN_TAGS{ airwindohhs::derez3::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    derez3_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(derez3_tilde);
