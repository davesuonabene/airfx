#include "c74_min.h"
#include "airfx.hpp"
#include "ambience/brightambience.hpp"

using namespace c74::min;

class brightambience_tilde : public airfx<brightambience_tilde, airwindohhs::brightambience::BrightAmbience<double>>
{
    atom m_about_text = symbol{ airwindohhs::brightambience::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::brightambience::k_name.data() };
    MIN_TAGS{ airwindohhs::brightambience::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    brightambience_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(brightambience_tilde);
