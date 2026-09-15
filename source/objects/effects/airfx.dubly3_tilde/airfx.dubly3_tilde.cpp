#include "c74_min.h"
#include "airfx.hpp"
#include "effects/dubly3.hpp"

using namespace c74::min;

class dubly3_tilde : public airfx<dubly3_tilde, airwindohhs::dubly3::Dubly3<double>>
{
    atom m_about_text = symbol{ airwindohhs::dubly3::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::dubly3::k_name.data() };
    MIN_TAGS{ airwindohhs::dubly3::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    dubly3_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(dubly3_tilde);
