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
