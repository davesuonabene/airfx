#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/atmospherebuss.hpp"

using namespace c74::min;

class atmospherebuss_tilde : public airfx<atmospherebuss_tilde, airwindohhs::atmospherebuss::AtmosphereBuss<double>>
{
    atom m_about_text = symbol{ airwindohhs::atmospherebuss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::atmospherebuss::k_name.data() };
    MIN_TAGS{ airwindohhs::atmospherebuss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    atmospherebuss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(atmospherebuss_tilde);
