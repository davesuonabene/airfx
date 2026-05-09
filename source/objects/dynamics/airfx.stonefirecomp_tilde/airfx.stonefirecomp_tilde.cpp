#include "c74_min.h"
#include "airfx.hpp"
#include "dynamics/stonefirecomp.hpp"

using namespace c74::min;

class stonefirecomp_tilde : public airfx<stonefirecomp_tilde, airwindohhs::stonefirecomp::StoneFireComp<double>>
{
    atom m_about_text = symbol{ airwindohhs::stonefirecomp::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::stonefirecomp::k_name.data() };
    MIN_TAGS{ airwindohhs::stonefirecomp::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    stonefirecomp_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(stonefirecomp_tilde);
