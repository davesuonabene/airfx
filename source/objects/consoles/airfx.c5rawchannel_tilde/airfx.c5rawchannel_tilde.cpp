#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/c5rawchannel.hpp"

using namespace c74::min;

class c5rawchannel_tilde : public airfx<c5rawchannel_tilde, airwindohhs::c5rawchannel::C5RawChannel<double>>
{
    atom m_about_text = symbol{ airwindohhs::c5rawchannel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::c5rawchannel::k_name.data() };
    MIN_TAGS{ airwindohhs::c5rawchannel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    c5rawchannel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(c5rawchannel_tilde);
