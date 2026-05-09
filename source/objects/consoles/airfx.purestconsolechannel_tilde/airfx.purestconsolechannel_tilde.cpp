#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/purestconsolechannel.hpp"

using namespace c74::min;

class purestconsolechannel_tilde : public airfx<purestconsolechannel_tilde, airwindohhs::purestconsolechannel::PurestConsoleChannel<double>>
{
    atom m_about_text = symbol{ airwindohhs::purestconsolechannel::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::purestconsolechannel::k_name.data() };
    MIN_TAGS{ airwindohhs::purestconsolechannel::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    purestconsolechannel_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(purestconsolechannel_tilde);
