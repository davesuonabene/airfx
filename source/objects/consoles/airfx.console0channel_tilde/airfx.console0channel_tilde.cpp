#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console0channel.hpp"

using namespace c74::min;

class console0channel_tilde : public airfx<console0channel_tilde, airwindohhs::console0channel::Console0Channel<double>>
{
    atom m_about_text = symbol{ airwindohhs::console0channel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console0channel::k_name.data() };
    MIN_TAGS{ airwindohhs::console0channel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console0channel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console0channel_tilde);
