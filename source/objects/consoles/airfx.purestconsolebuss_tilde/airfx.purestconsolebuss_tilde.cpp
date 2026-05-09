#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/purestconsolebuss.hpp"

using namespace c74::min;

class purestconsolebuss_tilde : public airfx<purestconsolebuss_tilde, airwindohhs::purestconsolebuss::PurestConsoleBuss<double>>
{
    atom m_about_text = symbol{ airwindohhs::purestconsolebuss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::purestconsolebuss::k_name.data() };
    MIN_TAGS{ airwindohhs::purestconsolebuss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    purestconsolebuss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(purestconsolebuss_tilde);
