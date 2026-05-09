#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8busshype.hpp"

using namespace c74::min;

class console8busshype_tilde : public airfx<console8busshype_tilde, airwindohhs::console8busshype::Console8BussHype<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8busshype::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8busshype::k_name.data() };
    MIN_TAGS{ airwindohhs::console8busshype::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8busshype_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8busshype_tilde);
