#pragma once
#include "c74_min.h"

using namespace c74::min;

template <typename TWrapper, typename TWrapped>
class airfx : public object<TWrapper>
    , public vector_operator<>
{
  protected:
    std::unique_ptr<TWrapped> m_wrapped{ std::make_unique<TWrapped>() };
    std::vector<std::unique_ptr<attribute<number, threadsafe::no, limit::clamp>>> m_airfx_parameters{};

  public:
    // Explicitly expose required methods from object_base/object to the wrapper
    // Use void* to avoid header conflicts with Max's t_object in some contexts
    void assign_instance(void* instance) {
        object<TWrapper>::assign_instance(reinterpret_cast<c74::max::t_object*>(instance));
    }

    void postinitialize() {
        object<TWrapper>::postinitialize();
    }

    void set_classname(const symbol s) {
        object<TWrapper>::set_classname(s);
    }

    // signature expected by wrap_as_max_external_common
    atoms try_call(const std::string& name, const atoms& args = {}) {
        return object<TWrapper>::try_call(name, args);
    }
    
    // Additional overload for the specific case in wrapper
    atoms try_call(const std::string& name, void* arg) {
        atoms as = { atom(arg) };
        return object<TWrapper>::try_call(name, as);
    }

    bool is_ui_class() const {
        return object<TWrapper>::is_ui_class();
    }

    airfx(const atoms& args) : object<TWrapper>()
    {
        auto attributes = this->attributes();
        if (attributes.empty()) {
            for (int i = 0; i < m_wrapped->kNumParameters; i++) {
                auto name = static_cast<std::string>(m_wrapped->get_parameter_name(i));
                m_airfx_parameters.push_back(std::make_unique<attribute<number, threadsafe::no, limit::clamp>>(
                    this,
                    m_wrapped->get_parameter_name(i).data(),
                    m_wrapped->get_parameter_default(i),
                    title{ m_wrapped->get_parameter_title(i).data() },
                    range{ 0.0, 1.0 },
                    setter{ [this, i](const atoms& args, const int inlet) -> atoms {
                        m_wrapped->set_parameter_value(i, args[0]);
                        return {};
                    } },
                    getter{
                        [this, i]() -> atoms {
                            return { m_wrapped->get_parameter_value(i) };
                        } }));

                attributes[name] = dynamic_cast<attribute_base*>(m_airfx_parameters.at(i).get());
            }
        }
    }

#ifdef AIRFX_NO_DOC_GEN
    MIN_FLAGS{ documentation_flags::do_not_generate };
#endif

    MIN_AUTHOR{ "Isabel Kaspriskie" };

    inlet<> in1{ this, "(signal) Input L" };
    inlet<> in2{ this, "(signal) Input R" };
    outlet<> out1{ this, "(signal) Output L", "signal" };
    outlet<> out2{ this, "(signal) Output R", "signal" };
    outlet<> dump_out{ this, "Dump outlet" };

    message<> m_dsp_setup{
        this,
        "dspsetup",
        [this](const atoms& args, const int inlet) -> atoms {
            m_wrapped->setSampleRate(this->samplerate());
            return {};
        }
    };

    void operator()(audio_bundle input, audio_bundle output) override
    {
        m_wrapped->process(input.samples(), output.samples(), input.frame_count());
    }
};
