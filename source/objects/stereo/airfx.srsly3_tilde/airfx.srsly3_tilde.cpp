#include "c74_min.h"
#include "airfx.hpp"
#include "stereo/srsly3.hpp"

using namespace c74::min;

class srsly3_tilde : public airfx<srsly3_tilde, airwindohhs::srsly3::Srsly3<double>>
{
    atom m_about_text = symbol{ airwindohhs::srsly3::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::srsly3::k_name.data() };
    MIN_TAGS{ airwindohhs::srsly3::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    srsly3_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(srsly3_tilde);
