#include "c74_min.h"
#include "airfx.hpp"
#include "bass/dubsub2.hpp"

using namespace c74::min;

class dubsub2_tilde : public airfx<dubsub2_tilde, airwindohhs::dubsub2::DubSub2<double>>
{
    atom m_about_text = symbol{ airwindohhs::dubsub2::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::dubsub2::k_name.data() };
    MIN_TAGS{ airwindohhs::dubsub2::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    dubsub2_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(dubsub2_tilde);
