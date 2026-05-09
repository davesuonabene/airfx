#include "c74_min.h"
#include "airfx.hpp"
#include "xyz-filters/ynotnotch.hpp"

using namespace c74::min;

class ynotnotch_tilde : public airfx<ynotnotch_tilde, airwindohhs::ynotnotch::YNotNotch<double>>
{
    atom m_about_text = symbol{ airwindohhs::ynotnotch::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::ynotnotch::k_name.data() };
    MIN_TAGS{ airwindohhs::ynotnotch::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    ynotnotch_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(ynotnotch_tilde);
