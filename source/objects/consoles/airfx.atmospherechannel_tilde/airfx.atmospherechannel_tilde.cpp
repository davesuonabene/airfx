#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/atmospherechannel.hpp"

using namespace c74::min;

class atmospherechannel_tilde : public airfx<atmospherechannel_tilde, airwindohhs::atmospherechannel::AtmosphereChannel<double>>
{
    atom m_about_text = symbol{ airwindohhs::atmospherechannel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::atmospherechannel::k_name.data() };
    MIN_TAGS{ airwindohhs::atmospherechannel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    atmospherechannel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(atmospherechannel_tilde);
