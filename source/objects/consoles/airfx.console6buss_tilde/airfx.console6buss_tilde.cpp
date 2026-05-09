#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console6buss.hpp"

using namespace c74::min;

class console6buss_tilde : public airfx<console6buss_tilde, airwindohhs::console6buss::Console6Buss<double>>
{
    atom m_about_text = symbol{ airwindohhs::console6buss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console6buss::k_name.data() };
    MIN_TAGS{ airwindohhs::console6buss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console6buss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console6buss_tilde);
