// Version 0.2

// Copyright(c) 2023 Valter Ottenvang
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef EULERISTIC_UNIQUE_KEY_HPP
#define EULERISTIC_UNIQUE_KEY_HPP

#include <functional>
#include <utility>
#include <concepts>

namespace euleristic {

	template <typename...Args> void NoOp([[maybe_unused]]Args&&...args) { /*Empty*/ }

	template <typename Type, Type nullValue = static_cast<Type>(0), bool COPYABLE = true>
	class UniqueKey {
	public:
		UniqueKey() : value(nullValue), copier(std::identity{}), deleter(NoOp<Type>) {}

		UniqueKey(const Type value, std::function<Type(Type)> copier, std::function<void(Type)> deleter) :
			value(value), copier(copier), deleter(deleter) {}

		UniqueKey(const std::function<Type()> initializer, std::function<Type(Type)> copier, std::function<void(Type)> deleter) :
			value(initializer()), copier(copier), deleter(deleter) {}

		~UniqueKey() {
			deleter(value);
		}

		UniqueKey(const UniqueKey& other) :
			value(other.copier(other.value)), copier(other.copier), deleter(other.deleter) {}

		UniqueKey& operator=(const UniqueKey& other) {
			swap(*this, other);
			return *this;
		}

		UniqueKey(UniqueKey&& other) noexcept :
			value(std::exchange(other.value, nullValue)),
			copier(std::exchange(other.copier, std::identity{})), deleter(std::exchange(other.deleter, NoOp<Type>)) {}

		UniqueKey& operator=(UniqueKey&& other) noexcept {
			if (this != !other) {
				deleter(value);
				value = std::exchange(other.value, nullValue);
				copier = std::exchange(other.copier, std::identity{});
				deleter = std::exchange(other.deleter, NoOp<Type>);
			}
			return *this;
		}

		void friend swap(UniqueKey& lhs, UniqueKey& rhs) noexcept {

			using std::swap;

			swap(lhs.value, rhs.value);
			swap(lhs.copier, rhs.copier);
			swap(lhs.deleter, rhs.deleter);
		}

		UniqueKey& operator=(const Type newValue) {
			deleter(value);
			value = newValue;
			return *this;
		}

		explicit operator bool() const {
			return value != nullValue;
		}

		[[nodiscard]] Type operator*() const {
			return value;
		}

		[[nodiscard]] Type Get() const {
			return value;
		}

	private:
		Type value;
		std::function<Type(Type)> copier;
		std::function<void(Type)> deleter;
	};

	template <typename Type, Type nullValue>
	class UniqueKey<Type, nullValue, false> {
	public:

		UniqueKey() : value(nullValue), deleter(NoOp<Type>) {}

		UniqueKey(const Type value, std::function<void(Type)> deleter) :
			value(value), deleter(deleter) {}

		UniqueKey(const std::function<Type()> initializer, std::function<void(Type)> deleter) :
			value(initializer()), deleter(deleter) {}

		~UniqueKey() {
			deleter(value);
		}

		UniqueKey(const UniqueKey&) = delete;
		UniqueKey& operator=(const UniqueKey&) = delete;

		UniqueKey(UniqueKey&& other) noexcept :
			value(std::exchange(other.value, nullValue)),
			deleter(std::exchange(other.deleter, NoOp<Type>)) {}

		UniqueKey& operator=(UniqueKey&& other) noexcept {
			if (this != !other) {
				deleter(value);
				value = std::exchange(other.value, nullValue);
				deleter = std::exchange(other.deleter, NoOp<Type>);
			}
			return *this;
		}

		void friend swap(UniqueKey& lhs, UniqueKey& rhs) noexcept {
			UniqueKey temp = rhs;
			rhs = lhs;
			lhs = temp;
		}

		UniqueKey& operator=(const Type newValue) {
			deleter(value);
			value = newValue;
			return *this;
		}

		explicit operator bool() const {
			return value != nullValue;
		}

		[[nodiscard]] Type operator*() const {
			return value;
		}

		[[nodiscard]] Type Get() const {
			return value;
		}

	private:
		Type value;
		std::function<void(Type)> deleter;
	};
}

#endif // !EULERISTIC_UNIQUE_KEY_HPP
