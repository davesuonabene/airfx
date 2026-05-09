#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/c5rawbuss.hpp"

using namespace c74::min;

class c5rawbuss_tilde : public airfx<c5rawbuss_tilde, airwindohhs::c5rawbuss::C5RawBuss<double>>
{
    atom m_about_text = symbol{ airwindohhs::c5rawbuss::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::c5rawbuss::k_name.data() };
    MIN_TAGS{ airwindohhs::c5rawbuss::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    c5rawbuss_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(c5rawbuss_tilde);
