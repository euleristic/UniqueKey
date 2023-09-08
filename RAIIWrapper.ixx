// Version 0.1

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

export module RAIIWrapper;

import std;

namespace euleristic {

	template <typename...Args> void NoOp([[maybe_unused]]Args&&...args) {}

	export template <typename Type, Type nullValue = static_cast<Type>(0), bool COPYABLE = true>
	class RAIIWrapper {
	public:
		RAIIWrapper() : value(nullValue), copier(std::identity{}), deleter(NoOp<Type>) {}

		RAIIWrapper(const Type value, std::function<Type(Type)> copier, std::function<void(Type)> deleter) :
			value(value), copier(copier), deleter(deleter) {}

		RAIIWrapper(const std::function<Type()> initializer, std::function<Type(Type)> copier, std::function<void(Type)> deleter) :
			value(initializer()), copier(copier), deleter(deleter) {}

		~RAIIWrapper() {
			deleter(value);
		}

		RAIIWrapper(const RAIIWrapper& other) :
			value(other.copier(other.value)), copier(other.copier), deleter(other.deleter) {}

		RAIIWrapper& operator=(const RAIIWrapper& other) {
			swap(*this, other);
			return *this;
		}

		RAIIWrapper(RAIIWrapper&& other) noexcept :
			value(std::exchange(other.value, nullValue)),
			copier(std::exchange(other.copier, std::identity{})), deleter(std::exchange(other.deleter, NoOp<Type>)) {}

		RAIIWrapper& operator=(RAIIWrapper&& other) noexcept {
			value = std::exchange(other.value, nullValue);
			copier = std::exchange(other.copier, std::identity{});
			deleter = std::exchange(other.deleter, NoOp<Type>);
			return *this;
		}

		void friend swap(RAIIWrapper& lhs, RAIIWrapper& rhs) noexcept {

			using std::swap;

			swap(lhs.value, rhs.value);
			swap(lhs.copier, rhs.copier);
			swap(lhs.deleter, rhs.deleter);
		}

		RAIIWrapper& operator=(const Type newValue) {
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

	export template <typename Type, Type nullValue>
	class RAIIWrapper<Type, nullValue, false> {
	public:

		RAIIWrapper() : value(nullValue), deleter(NoOp<Type>) {}

		RAIIWrapper(const Type value, std::function<void(Type)> deleter) :
			value(value), deleter(deleter) {}

		RAIIWrapper(const std::function<Type()> initializer, std::function<void(Type)> deleter) :
			value(initializer()), deleter(deleter) {}

		~RAIIWrapper() {
			deleter(value);
		}

		RAIIWrapper(const RAIIWrapper&) = delete;
		RAIIWrapper& operator=(const RAIIWrapper&) = delete;

		RAIIWrapper(RAIIWrapper&& other) noexcept :
			value(std::exchange(other.value, nullValue)),
			deleter(std::exchange(other.deleter, NoOp<Type>)) {}

		RAIIWrapper& operator=(RAIIWrapper&& other) noexcept {
			value = std::exchange(other.value, nullValue);
			deleter = std::exchange(other.deleter, NoOp<Type>);
			return *this;
		}

		void friend swap(RAIIWrapper& lhs, RAIIWrapper& rhs) noexcept {
			RAIIWrapper temp = rhs;
			rhs = lhs;
			lhs = temp;
		}

		RAIIWrapper& operator=(const Type newValue) {
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