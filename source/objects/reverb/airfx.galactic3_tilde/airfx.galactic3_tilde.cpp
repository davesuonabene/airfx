#include "c74_min.h"
#include "airfx.hpp"
#include "reverb/galactic3.hpp"

using namespace c74::min;

class galactic3_tilde : public airfx<galactic3_tilde, airwindohhs::galactic3::Galactic3<double>>
{
    atom m_about_text = symbol{ airwindohhs::galactic3::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::galactic3::k_name.data() };
    MIN_TAGS{ airwindohhs::galactic3::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    galactic3_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(galactic3_tilde);
