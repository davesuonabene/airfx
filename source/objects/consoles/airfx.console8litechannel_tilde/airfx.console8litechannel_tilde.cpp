#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8litechannel.hpp"

using namespace c74::min;

class console8litechannel_tilde : public airfx<console8litechannel_tilde, airwindohhs::console8litechannel::Console8LiteChannel<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8litechannel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8litechannel::k_name.data() };
    MIN_TAGS{ airwindohhs::console8litechannel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8litechannel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8litechannel_tilde);
