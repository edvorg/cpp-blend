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

#include <iostream>

#include <blend.hpp>

int main() {
	using std::cout;
	using std::endl;

	cout << "running blend tests" << endl;

	cout << blend::blend("hello, ", 1, " hello ", 2, 3) << endl;
	cout << blend::blend("hello, ", 1, " hello ", 2, 3).join("<delim>") << endl;
	cout << list(1, 2, 3).reverse(list()).toString() << endl;
	cout << "blend tests finished" << endl;

	return 0;
}
