#include "c74_min.h"
#include "airfx.hpp"
#include "reverb/kcathedral3.hpp"

using namespace c74::min;

class kcathedral3_tilde : public airfx<kcathedral3_tilde, airwindohhs::kcathedral3::kCathedral3<double>>
{
    atom m_about_text = symbol{ airwindohhs::kcathedral3::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::kcathedral3::k_name.data() };
    MIN_TAGS{ airwindohhs::kcathedral3::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    kcathedral3_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(kcathedral3_tilde);
