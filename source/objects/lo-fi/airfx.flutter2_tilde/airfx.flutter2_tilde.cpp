#include "c74_min.h"
#include "airfx.hpp"
#include "lo-fi/flutter2.hpp"

using namespace c74::min;

class flutter2_tilde : public airfx<flutter2_tilde, airwindohhs::flutter2::Flutter2<double>>
{
    atom m_about_text = symbol{ airwindohhs::flutter2::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::flutter2::k_name.data() };
    MIN_TAGS{ airwindohhs::flutter2::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    flutter2_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(flutter2_tilde);
