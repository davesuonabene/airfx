#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8bussout.hpp"

using namespace c74::min;

class console8bussout_tilde : public airfx<console8bussout_tilde, airwindohhs::console8bussout::Console8BussOut<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8bussout::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8bussout::k_name.data() };
    MIN_TAGS{ airwindohhs::console8bussout::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8bussout_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8bussout_tilde);
