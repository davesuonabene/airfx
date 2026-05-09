#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8channelout.hpp"

using namespace c74::min;

class console8channelout_tilde : public airfx<console8channelout_tilde, airwindohhs::console8channelout::Console8ChannelOut<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8channelout::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8channelout::k_name.data() };
    MIN_TAGS{ airwindohhs::console8channelout::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8channelout_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8channelout_tilde);
