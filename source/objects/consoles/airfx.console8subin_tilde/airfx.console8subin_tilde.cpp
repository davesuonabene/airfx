#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8subin.hpp"

using namespace c74::min;

class console8subin_tilde : public airfx<console8subin_tilde, airwindohhs::console8subin::Console8SubIn<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8subin::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8subin::k_name.data() };
    MIN_TAGS{ airwindohhs::console8subin::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8subin_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8subin_tilde);
