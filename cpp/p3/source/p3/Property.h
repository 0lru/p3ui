/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/
#include <functional>

namespace p3
{


    template<typename T>
    class Property
    {
    public:
        class Listener
        {
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
