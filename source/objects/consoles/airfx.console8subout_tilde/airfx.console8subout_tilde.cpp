#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8subout.hpp"

using namespace c74::min;

class console8subout_tilde : public airfx<console8subout_tilde, airwindohhs::console8subout::Console8SubOut<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8subout::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8subout::k_name.data() };
    MIN_TAGS{ airwindohhs::console8subout::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8subout_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8subout_tilde);
