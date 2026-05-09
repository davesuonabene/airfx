#include "c74_min.h"
#include "airfx.hpp"
#include "dithers/ditherfloat.hpp"

using namespace c74::min;

class ditherfloat_tilde : public airfx<ditherfloat_tilde, airwindohhs::ditherfloat::DitherFloat<double>>
{
    atom m_about_text = symbol{ airwindohhs::ditherfloat::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::ditherfloat::k_name.data() };
    MIN_TAGS{ airwindohhs::ditherfloat::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    ditherfloat_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(ditherfloat_tilde);
