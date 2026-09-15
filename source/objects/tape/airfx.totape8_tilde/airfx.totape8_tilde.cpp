#include "c74_min.h"
#include "airfx.hpp"
#include "tape/totape8.hpp"

using namespace c74::min;

class totape8_tilde : public airfx<totape8_tilde, airwindohhs::totape8::ToTape8<double>>
{
    atom m_about_text = symbol{ airwindohhs::totape8::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::totape8::k_name.data() };
    MIN_TAGS{ airwindohhs::totape8::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    totape8_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(totape8_tilde);
