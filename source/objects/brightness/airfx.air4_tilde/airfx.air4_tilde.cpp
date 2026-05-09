#include "c74_min.h"
#include "airfx.hpp"
#include "brightness/air4.hpp"

using namespace c74::min;

class air4_tilde : public airfx<air4_tilde, airwindohhs::air4::Air4<double>>
{
    atom m_about_text = symbol{ airwindohhs::air4::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::air4::k_name.data() };
    MIN_TAGS{ airwindohhs::air4::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    air4_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(air4_tilde);
