#include <iostream>
#include <set>
#include <random>
#include <cmath>
#include <iomanip>

template <class T> //шаблон класса, который будет давать возможность использовать оператор для разных классов
class MyProbabilityTest {
private:
	unsigned m_seed;
	int m_test_min, m_test_max;
	unsigned m_test_count;

public:
	MyProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): m_seed(seed), m_test_min(test_min), m_test_max(test_max), m_test_count(test_count) { }

	float operator()(T const &s) const {
		std::mt19937 rng(m_seed);
		std::uniform_int_distribution<int> dstr(m_test_min, m_test_max);
		unsigned good = 0;
		for (unsigned cnt = 0; cnt != m_test_count; ++cnt)
			if (s.contains(dstr(rng))) ++good;

		return static_cast<float>(good)/static_cast<float>(m_test_count);
	}
};

class DiscreteState { //дискретное(прерывистое) состояние -- задаётся одним элементом
private:
	int const state;

public:
	DiscreteState(int state) : state(state) { }

	DiscreteState(DiscreteState const& other) : state(other.state) { }

	bool contains(int s) const {
		return (s == state);
	}
};

class SegmentState { //состояние из сегмента -- задаётся массивом элементов
private:
	int const beg, end;

public:
	SegmentState() : beg(0), end(-1) { }
	SegmentState(int beg, int end) : beg(beg), end(end) { }

	SegmentState(SegmentState const& other) : beg(other.beg), end(other.end) { }

	bool contains(int s) const {
		return s >= beg && s <= end;
	}
};

class SetState { // состояние из множества -- показывает, является ли число элементом множества
private:
	std::set<int> const states;

public:
	SetState() : states() { }
	SetState(std::set<int> const& src) : states(src) { }

	SetState(SetState const& other) : states(other.states) { }

	bool contains(int s) const {
		return states.count(s) > 0;
	}
};

template <class A, class B>
class StateUnion { //состояние, как объединение двух других состояний
private:
	A const a; 
	B const b;

public:
	StateUnion(A a, B b) : a(a), b(b) { }

	StateUnion(StateUnion<A, B> const& other) : a(other.a), b(other.b) { }

	bool contains(int s) const {
		return a.contains(s) || b.contains(s);
	}
};

template <class A, class B>
class StateSubtraction { //состояние, как дополнение одного до другого (вычитание множеств)
private:
	A const a;
	B const b;

public:
	StateSubtraction(A a, B b) : a(a), b(b) { }

	StateSubtraction(StateSubtraction<A, B> const& other) : a(other.a), b(other.b) { }

	bool contains(int s) const {
		return a.contains(s) && !b.contains(s);
	}
};

template <class A, class B>
class StateIntersection { //пересечение двух состояний
private:
	A const a;
	B const b;

public:
	StateIntersection(A a, B b) : a(a), b(b) { }

	StateIntersection(StateIntersection<A, B> const& other) : a(other.a), b(other.b) { }

	bool contains(int s) const {
		return a.contains(s) && b.contains(s);
	}
};

class SegmentStateOmissions : public StateSubtraction<SegmentState, SetState> {
public:
	SegmentStateOmissions(SegmentState segmentState, SetState omissions) :
		StateSubtraction<SegmentState, SetState>::StateSubtraction(segmentState, omissions) { }
};

class SegmentStateAdditions : public StateUnion<SegmentState, SetState> {
public:
	SegmentStateAdditions(SegmentState segmentState, SetState additions) :
		StateUnion<SegmentState, SetState>::StateUnion(segmentState, additions) { }
};

class SegmentStateOmissionsAdditions : public StateSubtraction<SegmentStateAdditions, SetState> {
public:
	SegmentStateOmissionsAdditions(SegmentState segmentState, SetState omissions, SetState additions) :
		StateSubtraction<SegmentStateAdditions, SetState>::StateSubtraction(SegmentStateAdditions(segmentState, additions), omissions) { }
};
/* 
class ProbabilityTest {
private:
	unsigned seed;
	int test_min, test_max;
	unsigned test_count;

public:
	ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }

	float operator()(DiscreteState const &s) const {
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<int> dstr(test_min,test_max);
		unsigned good = 0;
		for (unsigned cnt = 0; cnt != test_count; ++cnt)
			if (s.contains(dstr(rng))) ++good;

		return static_cast<float>(good)/static_cast<float>(test_count);
	}
	float operator()(SegmentState const &s) const {
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<int> dstr(test_min,test_max);
		unsigned good = 0;
		for (unsigned cnt = 0; cnt != test_count; ++cnt)
			if (s.contains(dstr(rng))) ++good;

		return static_cast<float>(good)/static_cast<float>(test_count);
	}

	float operator()(SetState const &s) const {
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<int> dstr(test_min,test_max);
		unsigned good = 0;
		for (unsigned cnt = 0; cnt != test_count; ++cnt)
			if (s.contains(dstr(rng))) ++good;

		return static_cast<float>(good)/static_cast<float>(test_count);
	}
};
*/

class ProbabilityTest : public 	MyProbabilityTest<DiscreteState>,
						public 	MyProbabilityTest<SegmentState>,
						public 	MyProbabilityTest<SetState>,
						public 	MyProbabilityTest<SegmentStateOmissions>,
						public 	MyProbabilityTest<SegmentStateAdditions>,
						public 	MyProbabilityTest<SegmentStateOmissionsAdditions> {
public:
	ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count):
								MyProbabilityTest<DiscreteState>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SegmentState>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SetState>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SegmentStateOmissions>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SegmentStateAdditions>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SegmentStateOmissionsAdditions>::MyProbabilityTest(seed, test_min, test_max, test_count) { }
};

/* код без наследования, но короткий и рабочий
class ProbabilityTest {
private:
	unsigned seed;
	int test_min, test_max;
	unsigned test_count;

public:
	ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

	template <class T>
	float operator()(T const& s) const {
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<int> dstr(test_min, test_max);
		unsigned good = 0;
		for (unsigned cnt = 0; cnt != test_count; ++cnt)
			if (s.contains(dstr(rng))) ++good;

		return static_cast<float>(good) / static_cast<float>(test_count);
	}
};
*/

const int maxNumberTest = 1000;

template <class T>
void func1(T s) {
	for (int i = 0; i < maxNumberTest; ++i) {
		ProbabilityTest pt(10, 0, 100, i + 1);
		std::cout << std::fixed << std::setw(10) << pt(s) << '\n';
	}
	std::cout << '\n';
}

template <class T>
void func2(T s, int ss) {
	ProbabilityTest pt1(10, 0, 100, 1000000), pt2(10, 0, 200, 1000000), pt3(10, 0, 500, 1000000);
	std::cout << ss << ' ' << 101 << ' ' << std::fixed << std::setprecision(6) << pt1(s) << ' ' << ss / 101.0 << '\n';
	std::cout << ss << ' ' << 201 << ' ' << std::fixed << std::setprecision(6) << pt2(s) << ' ' << ss / 201.0 << '\n';
	std::cout << ss << ' ' << 501 << ' ' << std::fixed << std::setprecision(6) << pt3(s) << ' ' << ss / 501.0 << "\n\n";
}

template <class T>
void func3(T s) {
	func1(s);
	func2(s, 11);
}


int main(int argc, const char* argv[]) {
	/*
	DiscreteState d(1);
	SegmentState s(0,10);
	SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
	ProbabilityTest pt(10,0,100,100000);
	std::cout << pt(d) << std::endl;
	std::cout << pt(s) << std::endl;
	std::cout << pt(ss) << std::endl;
	*/

	func1(SegmentState(0, 10));
	func1(SetState({ 1, 3, 5, 7, 11, 23, 48, 57, 60, 90, 99 }));

	func2(SegmentState(0, 10), 11);
	func2(SegmentState(0, 20), 21);
	func2(SegmentState(0, 50), 51);

	func3(SegmentStateAdditions(SegmentState(0, 9), SetState({ 15 })));
	func3(SegmentStateOmissions(SegmentState(0, 11), SetState({ 5 })));
	func3(SegmentStateOmissionsAdditions(SegmentState(0, 10), SetState({ 5 }), SetState({ 15 })));

	return 0;
}