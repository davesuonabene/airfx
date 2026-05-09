#include "c74_min.h"
#include "airfx.hpp"
#include "filter/parametric.hpp"

using namespace c74::min;

class parametric_tilde : public airfx<parametric_tilde, airwindohhs::parametric::Parametric<double>>
{
    atom m_about_text = symbol{ airwindohhs::parametric::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::parametric::k_name.data() };
    MIN_TAGS{ airwindohhs::parametric::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    parametric_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(parametric_tilde);
