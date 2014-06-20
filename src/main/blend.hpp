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

#include <tuple>
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
	Blend(T&& t, U&& ... u) : head{std::forward<T>(t)}, tail{std::forward<U>(u) ... } {}


	bool empty() const { return false; }
	bool nonEmpty() const { return !empty(); }


	operator string() const { return cat(); }
	string cat() const { return join(""); }
	ostream& cat(ostream& s) const { return join("", s); }


	string join(const string& d) const { auto s = ss(); join(d, s); return s.str(); }
	ostream& join(const string& d, ostream& s) const {
		s << head; if (tail.nonEmpty()) s << d; return tail.join(d, s);
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
	bool empty() const { return true; }
	bool nonEmpty() const { return !empty(); }


	operator string() const { return {}; }
	string cat() const { return *this; }
	ostream& cat(ostream& s) const { return s; }


	string join(const string&) const { return {}; }
	ostream& join(const string&, ostream& s) const { return s; }

private:
	ostringstream ss() const { return ostringstream{}; }
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
