#include "c74_min.h"
#include "airfx.hpp"
#include "distortion/zoom.hpp"

using namespace c74::min;

class zoom_tilde : public airfx<zoom_tilde, airwindohhs::zoom::Zoom<double>>
{
    atom m_about_text = symbol{ airwindohhs::zoom::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::zoom::k_name.data() };
    MIN_TAGS{ airwindohhs::zoom::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    zoom_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(zoom_tilde);
