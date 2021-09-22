#include <iostream>
#include <set>
#include <random>

// My includes
#include <cmath>
#include <iomanip>



/* My code begins */

/* This code uses inheritance, extension, and redefinition. */

/*
template <class T>
class MyProbabilityTest {
private:
    unsigned m_seed;
    int m_test_min, m_test_max;
    unsigned m_test_count;
    
public:
    MyProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): m_seed(seed), m_test_min(test_min), m_test_max(test_max), m_test_count(test_count) { }
    
    float func(T const &s) const {
        std::default_random_engine rng(m_seed);
        std::uniform_int_distribution<int> dstr(m_test_min, m_test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != m_test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;
        
        return static_cast<float>(good)/static_cast<float>(m_test_count);
    }
};
*/

/* My code ends */



/* Added constructors for classes to original code */

class DiscreteState {
private:
    int const state;

public:
    DiscreteState(int state): state(state) { }
    
    /* This one was added */
    DiscreteState(DiscreteState const& other): state(other.state) { }

    bool contains(int s) const {
        return s == state;
    }
};

class SegmentState {
private:
    int const beg, end;
    
public:
    SegmentState(): beg(0), end(-1) { }
    SegmentState(int beg, int end): beg(beg), end(end) { }
    
    /* This one was added */
    SegmentState(SegmentState const& other): beg(other.beg), end(other.end) { }
    
    bool contains(int s) const {
        return s >= beg && s <= end;
    }
};

class SetState {
private:
    std::set<int> const states;
    
public:
    SetState(): states() { }
    SetState(std::set<int> const &src): states(src) { }
    
    /* This one was added */
    SetState(SetState const& other): states(other.states) { }
    
    bool contains(int s) const {
        return states.count(s) > 0;
    }
};



/* My code begins */

/* New classes. */

template <class A, class B>
class StateUnion {
private:
	A const a;
	B const b;

public:
	StateUnion(A a, B b): a(a), b(b) { }
	
	StateUnion(StateUnion<A, B> const& other): a(other.a), b(other.b) { }
	
	bool contains(int s) const {
		return a.contains(s) || b.contains(s);
	}
};

template <class A, class B>
class StateSubtraction {
private:
	A const a;
	B const b;

public:
	StateSubtraction(A a, B b): a(a), b(b) { }
	
	StateSubtraction(StateSubtraction<A, B> const& other): a(other.a), b(other.b) { }
	
	bool contains(int s) const {
		return a.contains(s) && !b.contains(s);
	}
};

template <class A, class B>
class StateIntersection {
private:
	A const a;
	B const b;

public:
	StateIntersection(A a, B b): a(a), b(b) { }
	
	StateIntersection(StateIntersection<A, B> const& other): a(other.a), b(other.b) { }
	
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

/* My code ends */



/* This code is original, but unnessesary. */

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



/* My code begins */

/* This code uses inheritance, extension, and redefinition. */

/*
class ProbabilityTest : public 	MyProbabilityTest<DiscreteState>, 
						public 	MyProbabilityTest<SegmentState>, 
						public 	MyProbabilityTest<SetState>,
						public 	MyProbabilityTest<SegmentStateOmissions>,
						public 	MyProbabilityTest<SegmentStateAdditions>,
						public 	MyProbabilityTest<SegmentStateOmissionsAdditions>,
						public 	MyProbabilityTest<StateUnion>,
						public 	MyProbabilityTest<StateSubtraction>,
						public 	MyProbabilityTest<StateIntersection> {
public:
	ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): 
								MyProbabilityTest<DiscreteState>::MyProbabilityTest(seed, test_min, test_max, test_count), 
								MyProbabilityTest<SegmentState>::MyProbabilityTest(seed, test_min, test_max, test_count), 
								MyProbabilityTest<SetState>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SegmentStateOmissions>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SegmentStateAdditions>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<SegmentStateOmissionsAdditions>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<StateUnion>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<StateSubtraction>::MyProbabilityTest(seed, test_min, test_max, test_count),
								MyProbabilityTest<StateIntersection>::MyProbabilityTest(seed, test_min, test_max, test_count) { }

	float operator()(DiscreteState const &s) const {
		return MyProbabilityTest<DiscreteState>::func(s);
	}
	
	float operator()(SegmentState const &s) const {
		return MyProbabilityTest<SegmentState>::func(s);
	}
	
	float operator()(SetState const &s) const {
		return MyProbabilityTest<SetState>::func(s);
	}
	
	float operator()(SegmentStateOmissions const &s) const {
		return MyProbabilityTest<SegmentStateOmissions>::func(s);
	}
	
	float operator()(SegmentStateAdditions const &s) const {
		return MyProbabilityTest<SegmentStateAdditions>::func(s);
	}
	
	float operator()(SegmentStateOmissionsAdditions const &s) const {
		return MyProbabilityTest<SegmentStateOmissionsAdditions>::func(s);
	}
	
	float operator()(StateUnion const &s) const {
		return MyProbabilityTest<StateUnion>::func(s);
	}
	
	float operator()(StateSubtraction const &s) const {
		return MyProbabilityTest<StateSubtraction>::func(s);
	}
	
	float operator()(StateIntersection const &s) const {
		return MyProbabilityTest<StateIntersection>::func(s);
	}
};
*/

/* My code ends */





/* My code begins */

/* This code is very short, but doesn't use inheritance, extension, and redefinition. This code is unsafe, 'cause you don't know, what is T. */
 
class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;
    
public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): 	seed(seed), 
																						test_min(test_min), 
																						test_max(test_max), 
																						test_count(test_count) { }
    
    template <class T>
    float operator()(T const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;
        
        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

/* My code ends */




/* My code begins */

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
void func2(T s) {
	ProbabilityTest pt1(10, 0, 100, 1000), pt2(10, 0, 200, 1000), pt3(10, 0, 500, 1000);
	std::cout << std::fixed << std::setw(10) << pt1(s) << '\n';
	std::cout << std::fixed << std::setw(10) << pt2(s) << '\n';
	std::cout << std::fixed << std::setw(10) << pt3(s) << '\n\n';
}

template <class T>
void func3(T s) {
	func1(s);
	func2(s);
}

/* My code ends */



int main(int argc, const char * argv[]) {
	/*
    DiscreteState d(1);
    SegmentState s(0,10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    ProbabilityTest pt(10,0,100,100000);
    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(ss) << std::endl;
    */
    
    /* Functions, answering the questions */
    
	func1(SegmentState(0, 10));
	func1(SetState({1, 3, 5, 7, 23, 48, 57, 60, 90, 99}));
	
	func2(SegmentState(0, 10));
	func2(SegmentState(0, 20));
	func2(SegmentState(0, 50));
	
	func3(SegmentStateAdditions(SegmentState(0, 9), SetState({15})));
	func3(SegmentStateOmissions(SegmentState(0, 11), SetState({5})));
	func3(SegmentStateOmissionsAdditions(SegmentState(0, 10), SetState({5}), SetState({15})));
	
    return 0;
}
