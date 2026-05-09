#include "c74_min.h"
#include "airfx.hpp"
#include "consoles/console8channelin.hpp"

using namespace c74::min;

class console8channelin_tilde : public airfx<console8channelin_tilde, airwindohhs::console8channelin::Console8ChannelIn<double>>
{
    atom m_about_text = symbol{ airwindohhs::console8channelin::k_long_description.data() };

  public:
    MIN_DESCRIPTION{ airwindohhs::console8channelin::k_name.data() };
    MIN_TAGS{ airwindohhs::console8channelin::k_tags.data() };

    message<> m_about{
        this,
        "about",
        description{ "Get the original Airwindows 'about' text for this object" },
        [this](const atoms& args, const int inlet) -> atoms {
            dump_out.send({"about", m_about_text});
            return {};
        }
    };

    console8channelin_tilde(const atoms& args = {})
        : airfx(args)
    {
    }
};

MIN_EXTERNAL(console8channelin_tilde);
