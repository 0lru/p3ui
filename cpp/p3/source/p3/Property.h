#include <functional>

namespace p3 {

template <typename T>
class Property {
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void on_property_changed() = 0;
    };

    using Getter = std::function<T()>;
    using Setter = std::function<void(T)>;

    Property(std::string name, Getter, Setter);

    std::string const& name() const;

    void T get() const;
    void set(T);
    void notify();

    void add_listener(std::shared_ptr<Listener> const&);
    void remove_listener(std::shared_ptr<Listener> const&);

private:
    std::shared_ptr<void> _tracked;
    std::string _name;
    Getter _getter;
    Setter _setter;
    std::vector<std::weak_ptr<Listener>> _listener;
};

}
