#include "c74_min.h"
#include "airfx.hpp"
#include "filter/isolator3.hpp"

using namespace c74::min;

class isolator3_tilde : public airfx<isolator3_tilde, airwindohhs::isolator3::Isolator3<double>>
{
    atom m_about_text = symbol{ airwindohhs::isolator3::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::isolator3::k_name.data() };
    MIN_TAGS{ airwindohhs::isolator3::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    isolator3_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(isolator3_tilde);
