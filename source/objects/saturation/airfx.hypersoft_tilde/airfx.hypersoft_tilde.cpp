#include "c74_min.h"
#include "airfx.hpp"
#include "saturation/hypersoft.hpp"

using namespace c74::min;

class hypersoft_tilde : public airfx<hypersoft_tilde, airwindohhs::hypersoft::Hypersoft<double>>
{
    atom m_about_text = symbol{ airwindohhs::hypersoft::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::hypersoft::k_name.data() };
    MIN_TAGS{ airwindohhs::hypersoft::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    hypersoft_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(hypersoft_tilde);
