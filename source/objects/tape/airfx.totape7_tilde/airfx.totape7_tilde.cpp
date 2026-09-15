#include "c74_min.h"
#include "airfx.hpp"
#include "tape/totape7.hpp"

using namespace c74::min;

class totape7_tilde : public airfx<totape7_tilde, airwindohhs::totape7::ToTape7<double>>
{
    atom m_about_text = symbol{ airwindohhs::totape7::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::totape7::k_name.data() };
    MIN_TAGS{ airwindohhs::totape7::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    totape7_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(totape7_tilde);
