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

#ifndef BLEND_SRC_MAIN_LIST_HPP
#define BLEND_SRC_MAIN_LIST_HPP

namespace blend {

template <typename ... T> class List;

template <typename T, typename ... U> class List<T, U ...> {
	using string = std::string;
	using ostringstream = std::ostringstream;
	using Head = T;
	using Tail = List<U ...>;

	const Head head;
	const Tail tail;

public:
	constexpr List(T&& t, U&& ... u) :
		head(std::forward<T>(t)), tail{std::forward<U>(u) ...} {}
	constexpr List(T&& t, const List<U ...>& l) :
		head(std::forward<T>(t)), tail{l} {}
	constexpr List(T&& t, List<U ...>&& l) :
		head(std::forward<T>(t)), tail{std::move(l)} {}

	static constexpr bool isEmpty() { return false; }
	static constexpr bool nonEmpty() { return !isEmpty(); }

	/// for debug purposes only
	string toString() const {
		ostringstream s{};
		s << head;
		if (tail.nonEmpty()) s << ", ";
		return s.str() + tail.toString();
	}

	constexpr const Head& getHead() { return head; }
	constexpr const Tail& getTail() { return tail; }

	template <typename V> constexpr List<V, T, U ...> prepend(V&& v) const {
		return List<V, T, U ...>(std::forward<V>(v), *this);
	}

	template <typename ... V> constexpr auto reverse(const List<V ...>& acc) const {
		return tail.reverse(acc.prepend(head));
	}
};

template <> class List<> {
	using string = std::string;

public:

	constexpr List() = default;

	static constexpr bool isEmpty() { return true; }
	static constexpr bool nonEmpty() { return !isEmpty(); }

	/// for debug purposes only
	string toString() const { return ""; }

	template <typename V> constexpr List<V> prepend(V&& v) const {
		return List<V>(std::forward<V>(v));
	}

	template <typename ... V> constexpr auto reverse(const List<V ...>& acc) const {
		return acc;
	}
};

template <typename ... T> constexpr auto list(T&& ... t) {
	return List<typename std::decay<T>::type ...>(std::forward<T>(t) ...);
}

} // namespace blend

#endif // BLEND_SRC_MAIN_LIST_HPP
