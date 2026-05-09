#include "c74_min.h"
#include "airfx.hpp"
#include "reverb/crunchcoat.hpp"

using namespace c74::min;

class crunchcoat_tilde : public airfx<crunchcoat_tilde, airwindohhs::crunchcoat::CrunchCoat<double>>
{
    atom m_about_text = symbol{ airwindohhs::crunchcoat::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::crunchcoat::k_name.data() };
    MIN_TAGS{ airwindohhs::crunchcoat::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    crunchcoat_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(crunchcoat_tilde);
