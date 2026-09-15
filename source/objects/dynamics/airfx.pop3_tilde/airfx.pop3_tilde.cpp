#include "c74_min.h"
#include "airfx.hpp"
#include "dynamics/pop3.hpp"

using namespace c74::min;

class pop3_tilde : public airfx<pop3_tilde, airwindohhs::pop3::Pop3<double>>
{
    atom m_about_text = symbol{ airwindohhs::pop3::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::pop3::k_name.data() };
    MIN_TAGS{ airwindohhs::pop3::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    pop3_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(pop3_tilde);
