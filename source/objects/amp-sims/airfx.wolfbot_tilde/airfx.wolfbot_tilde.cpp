#include "c74_min.h"
#include "airfx.hpp"
#include "amp-sims/wolfbot.hpp"

using namespace c74::min;

class wolfbot_tilde : public airfx<wolfbot_tilde, airwindohhs::wolfbot::Wolfbot<double>>
{
    atom m_about_text = symbol{ airwindohhs::wolfbot::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::wolfbot::k_name.data() };
    MIN_TAGS{ airwindohhs::wolfbot::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    wolfbot_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(wolfbot_tilde);
