// BLEND_LICENSE_BEGIN
/*

Blend LGPL Source Code
Copyright (C) 2014 Edward Knyshov.

This file is part of the Blend LGPL Source Code (Blend Source Code).

Blend Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Blend Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Blend Source Code.  If not, see <http://www.gnu.org/licenses/>.

Original Blend Source Code repository can be found at https://github.com/edvorg/cpp-blend

*/
// BLEND_LICENSE_END

#ifndef BLEND_SRC_MAIN_BLEND_HPP
#define BLEND_SRC_MAIN_BLEND_HPP

#include <utility>
#include <string>
#include <sstream>
#include <stdexcept>

namespace blend {

class BlendDynamic {
	using ostream = std::ostream;
	using ostringstream = std::ostringstream;
	using string = std::string;

public:
	virtual string elem(size_t i) const = 0;
	virtual ostream& elem(size_t i, ostream& s) const = 0;

	template <class V> string format(const V& f) const {
		ostringstream s{};
		format(std::begin(f), std::end(f), s);
		return s.str();
	}

	template <class V> ostream& format(V&& b, V&& e, ostream& s) const {
		if (b == e || *b == '\0') return s;

		auto pe = b + 1;

		if (*b == '$') {
			auto id = 0;

			for (; pe != e && ('0' <= *pe && *pe <= '9'); ++pe)
				id = id * 10 + (*pe - '0');

			elem(id, s);
		}
		else {
			for (; pe != e && *pe != '$' && *pe != '\0'; ++pe);
			s << string(b, pe);
		}

		return format(pe, e, s);
	}
};

template <typename ... T> class Blend;

template <typename ... T> constexpr Blend<typename std::decay<T>::type ...> blend(T&& ... t) {
	return Blend<typename std::decay<T>::type ...>{std::forward<T>(t) ...};
}

template <typename ... T> std::string cat(T&& ... t) {
	return blend(std::forward<T>(t) ...).cat();
}

template <typename ... T, typename U> std::string join(U&& u, T&& ... t) {
	return blend(std::forward<T>(t) ...).join(std::forward<U>(u));
}

template <typename ... T, typename U, typename V, typename W> std::string wrap(
	U&& u, V&& v, W&& w, T&& ... t) {
	return blend(std::forward<T>(t) ...).wrap(std::forward<U>(u),
											  std::forward<V>(v),
											  std::forward<W>(w));
}

template <typename ... T, typename U> std::string format(U&& u, T&& ... t) {
	return blend(std::forward<T>(t) ...).format(std::forward<U>(u));
}

template <typename T, typename ... U> class Blend<T, U ...> : public BlendDynamic {
	using ostream = std::ostream;
	using ostringstream = std::ostringstream;
	using string = std::string;
	using Head = T;
	using Tail = Blend<U ...>;

public:
	Blend(T&& t, U&& ... u) : head(std::forward<T>(t)), tail{std::forward<U>(u) ...} {}
	Blend(const T& t, const U& ... u) : head(t), tail{u ...} {}

	static constexpr bool isEmpty() { return false; }
	static constexpr bool nonEmpty() { return !isEmpty(); }
	static constexpr size_t size() { return 1 + Tail::size(); }

	virtual string elem(size_t i) const override {
		ostringstream s{};
		elem(i, s);
		return s.str();
	}
	virtual ostream& elem(size_t i, ostream& s) const override {
		if (size() < i)
			throw std::logic_error(
				blend::format("index out of bounds, size: $1, index $2", size, i));
		else if (i == 0) cat(s);
		else if (i == 1) s << head;
		else tail.elem(i - 1, s);
		return s;
	}

	operator string() const { return cat(); }
	string cat() const { return join(""); }
	ostream& cat(ostream& s) const { return join("", s); }

	template <class V> string join(V&& d) const {
		ostringstream s{}; join(std::forward<V>(d), s); return s.str();
	}
	template <class V> ostream& join(V&& d, ostream& s) const {
		s << head; if (tail.nonEmpty()) s << d; return tail.join(std::forward<V>(d), s);
	}

	template <class V, class W, class Y> string wrap(V&& b, W&& d, Y&& a) const {
		ostringstream s{};
		wrap(std::forward<V>(b), std::forward<W>(d), std::forward<Y>(a), s);
		return s.str();
	}
	template <class V, class W, class Y> ostream& wrap(V&& b, W&& d, Y&& a, ostream& s) const {
		s << std::forward<V>(b);
		join(std::forward<W>(d), s);
		s << std::forward<Y>(a);
		return s;
	}

private:
	Head head;
	Tail tail;
};


template <> class Blend<> : public BlendDynamic {
	using ostream = std::ostream;
	using ostringstream = std::ostringstream;
	using string = std::string;

public:
	static constexpr bool isEmpty() { return true; }
	static constexpr bool nonEmpty() { return !isEmpty(); }
	static constexpr size_t size() { return 0; }

	virtual string elem(size_t i) const override {
		ostringstream s{};
		elem(i, s);
		return s.str();
	}
	virtual ostream& elem(size_t i, ostream& s) const override {
		if (size() < i)
			throw std::logic_error(
				blend::format("index out of bounds, size: $1, index $2", size, i));
		return s;
	}

	operator string() const { return {}; }
	string cat() const { return *this; }
	ostream& cat(ostream& s) const { return s; }

	template <class T> string join(T&&) const { return {}; }
	template <class T> ostream& join(T&&, ostream& s) const { return s; }

	template <class T, class U, class V> string wrap(T&& b, U&& d, V&& a) const {
		ostringstream s{};
		wrap(std::forward<T>(b), std::forward<U>(d), std::forward<V>(a), s);
		return s.str();
	}
	template <class T, class U, class V> ostream& wrap(
		T&& b, U&& d, V&& a, ostream& s) const {
		s << std::forward<T>(b) << std::forward<V>(a);
		return s;
	}
};

template <typename ... T> std::ostream& operator<< (std::ostream& s, const Blend<T ...>& b) {
	s << b.cat();
	return s;
}

} // namespace blend

#endif // BLEND_SRC_MAIN_BLEND_HPP
