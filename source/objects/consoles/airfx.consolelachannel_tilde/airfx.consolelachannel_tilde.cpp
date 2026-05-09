#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/consolelachannel.hpp"

using namespace c74::min;

class consolelachannel_tilde : public airfx<consolelachannel_tilde, airwindohhs::consolelachannel::ConsoleLAChannel<double>>
{
    atom m_about_text = symbol{ airwindohhs::consolelachannel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::consolelachannel::k_name.data() };
    MIN_TAGS{ airwindohhs::consolelachannel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    consolelachannel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(consolelachannel_tilde);
