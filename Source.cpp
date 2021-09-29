#include <iostream>
#include <set>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <algorithm>

class State {
public:
	bool virtual contains(int s) const = 0;
	State() { }
	~State() { }
};

class DiscreteState : public State {
private:
	int const state;

public:
	DiscreteState(int state) : state(state) { }

	bool contains(int s) const override {
		return (s == state);
	}
};

class SegmentState : public State {
private:
	int const beg, end;

public:
	SegmentState(int beg, int end) : beg(beg), end(end) { }

	bool contains(int s) const override {
		return s >= beg && s <= end;
	}
};

class SetState : public State {
private:
	std::set<int> const states;

public:
	SetState() : states() { }
	SetState(std::set<int> const& src) : states(src) { }

	bool contains(int s) const override {
		return states.count(s) > 0;
	}
};

class StateUnion : public State {
private:
	State const& a;
	State const& b;

public:
	StateUnion(State const& a, State const& b) : a(a), b(b) { }

	bool contains(int s) const override {
		return a.contains(s) || b.contains(s);
	}
};

class StateSubtraction : public State {
private:
	State const& a;
	State const& b;

public:
	StateSubtraction(State const& a, State const& b) : a(a), b(b) { }

	bool contains(int s) const override {
		return a.contains(s) && !b.contains(s);
	}
};

class StateIntersection : public State {
private:
	State const& a;
	State const& b;

public:
	StateIntersection(State const& a, State const& b) : a(a), b(b) { }

	bool contains(int s) const override {
		return a.contains(s) && b.contains(s);
	}
};

class ProbabilityTest {
private:
	unsigned seed;
	int test_min, test_max;
	unsigned test_count;
public:
	ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }
	float operator()(State const& s) const {
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<int> dstr(test_min, test_max);
		unsigned good = 0;
		for (unsigned cnt = 0; cnt != test_count; ++cnt)
			if (s.contains(dstr(rng))) ++good;
		return static_cast<float>(good) / static_cast<float>(test_count);
	}
};

bool test(State& st, int t1, int t2, int t3) {
	return !st.contains(t1) && st.contains(t2) && !st.contains(t3);
}

int maxNumberTest = 1000;

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

void converge(State& s, int k)
{
	std::default_random_engine rng(k);
	std::uniform_int_distribution<int> dstr(0, 100);
	double epsilon = 0.00001;

	double old_prob = 0;
	double new_prob = 1;

	int num = 1000;

	while (std::abs(old_prob - new_prob) > epsilon)
	{
		int good = 0;
		for (int i = 0; i < num; i++)
		{
			good += (s.contains(dstr(rng))) ? 1 : 0;
		}
		old_prob = new_prob;
		new_prob = (double)(good) / (double)(num);
		num += 1000;
	}
	maxNumberTest = std::max(maxNumberTest, num);
	std::cout << "This thing converges at " << num << '\n';
}


int main(int argc, const char* argv[]) {

	SegmentState st01(5, 19);
	DiscreteState st02(3);
	SetState st03({ 1, 3, 5, 7 });
	StateUnion st04(st01, st02);
	StateSubtraction st05(st01, st03);
	StateIntersection st06(st01, st03);

	if (!test(st01, 4, 5, 20)) std::cout << "Problem with SegmentState\n";
	if (!test(st02, 2, 3, 4)) std::cout << "Problem with DiscreteState\n";
	if (!test(st03, 4, 5, 8)) std::cout << "Problem with SetState\n";
	if (!test(st04, 4, 3, 20)) std::cout << "Problem with StateUnion\n";
	if (!test(st05, 5, 6, 7)) std::cout << "Problem with StateSubtraction\n";
	if (!test(st06, 4, 5, 6)) std::cout << "Problem with StateIntersection\n";

	func1(SegmentState(0, 10));
	func1(SetState({ 1, 3, 5, 7, 11, 23, 48, 57, 60, 90, 99 }));

	func2(SegmentState(0, 10), 11);
	func2(SegmentState(0, 20), 21);
	func2(SegmentState(0, 50), 51);

	func3(StateUnion(SegmentState(0, 9), DiscreteState(11)));
	func3(StateSubtraction(SegmentState(0, 11), DiscreteState(5)));
	func3(StateIntersection(SegmentState(0, 11), SegmentState(1, 12)));

	std::vector<State*> States;
	States.push_back(&st01);
	States.push_back(&st02);
	States.push_back(&st03);
	States.push_back(&st04);
	States.push_back(&st05);
	States.push_back(&st06);

	for (auto i : States) {
		converge(*i, 775);
	}



	converge(st01, 579);

	std::cout << maxNumberTest;

	return 0;
}