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
#pragma once

#include <mutex>
#include <memory>
#include <stdexcept>

namespace p3
{

    class Synchronizable
    {
    public:
        virtual ~Synchronizable() = default;

        virtual void synchronize_with(Synchronizable&);
        virtual void release();

        [[nodiscard]] std::unique_ptr<std::lock_guard<std::recursive_mutex>> lock();
        [[nodiscard]] std::unique_ptr<std::lock_guard<std::recursive_mutex>> lock() const;

        std::shared_ptr<std::recursive_mutex> const& recursive_mutex() const;

    private:
        std::shared_ptr<std::recursive_mutex> _mutex = std::make_unique<std::recursive_mutex>();
    };

}
