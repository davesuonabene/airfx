#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8channelhype.hpp"

using namespace c74::min;

class console8channelhype_tilde : public airfx<console8channelhype_tilde, airwindohhs::console8channelhype::Console8ChannelHype<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8channelhype::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8channelhype::k_name.data() };
    MIN_TAGS{ airwindohhs::console8channelhype::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8channelhype_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8channelhype_tilde);
