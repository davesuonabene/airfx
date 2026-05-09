#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console9channel.hpp"

using namespace c74::min;

class console9channel_tilde : public airfx<console9channel_tilde, airwindohhs::console9channel::Console9Channel<double>>
{
    atom m_about_text = symbol{ airwindohhs::console9channel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console9channel::k_name.data() };
    MIN_TAGS{ airwindohhs::console9channel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console9channel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console9channel_tilde);
