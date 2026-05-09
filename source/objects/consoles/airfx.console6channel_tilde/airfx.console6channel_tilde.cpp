#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console6channel.hpp"

using namespace c74::min;

class console6channel_tilde : public airfx<console6channel_tilde, airwindohhs::console6channel::Console6Channel<double>>
{
    atom m_about_text = symbol{ airwindohhs::console6channel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console6channel::k_name.data() };
    MIN_TAGS{ airwindohhs::console6channel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console6channel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console6channel_tilde);
