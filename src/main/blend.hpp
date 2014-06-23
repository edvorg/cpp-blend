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

namespace blend {

template <typename ... T> class Blend;

template <typename T, typename ... U> class Blend<T, U ...> {
	using ostream = std::ostream;
	using ostringstream = std::ostringstream;
	using string = std::string;

public:
	Blend(T&& t, U&& ... u) : head(std::forward<T>(t)), tail{std::forward<U>(u) ... } {}

	static constexpr bool isEmpty() { return false; }
	static constexpr bool nonEmpty() { return !isEmpty(); }

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
	ostringstream ss() const { return ostringstream{}; }

	T head;
	Blend<U ...> tail;
};


template <> class Blend<> {
	using ostream = std::ostream;
	using ostringstream = std::ostringstream;
	using string = std::string;

public:
	static constexpr bool isEmpty() { return true; }
	static constexpr bool nonEmpty() { return !isEmpty(); }

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
	template <class T, class U, class V> ostream& wrap(T&& b, U&& d, V&& a, ostream& s) const {
		s << std::forward<T>(b) << std::forward<V>(a);
		return s;
	}
};


template <typename ... T> Blend<typename std::decay<T>::type ...> blend(T&& ... t) {
	return Blend<typename std::decay<T>::type ...>{std::forward<T>(t) ...};
}

template <typename ... T> std::ostream& operator<< (std::ostream& s, const Blend<T ...>& b) {
	s << b.cat();
	return s;
}

} // namespace blend

#endif // BLEND_SRC_MAIN_BLEND_HPP
