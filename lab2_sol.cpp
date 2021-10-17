#include <iostream>
#include <string>
#include <typeinfo>

class Handler
{
public:
	virtual void* data() = 0;
	virtual void const* data() const = 0;
	virtual std::type_info const& getType() const = 0;

	virtual ~Handler() {}
};

template<typename T>
class TypicalHandler : public Handler
{
public:
	TypicalHandler(T val)
		: val_(val)
	{}

	void* data() override { return static_cast<void*>(&val_); }

	void const* data() const override { return static_cast<void const*>(&val_); }

	std::type_info const& getType() const override { return typeid(val_); }

private:
	T val_;
};

class Any
{
public:
	Any()
		: data_(nullptr)
	{}

	template<typename T>
	Any(T val)
		: data_(new TypicalHandler<T>(val))
	{}

	template<typename T>
	Any(const Any& ref)
	{
		delete[] data_;
		data_ = new TypicalHandler<T>(ref.as<T>());
	}

	~Any() { delete data_; }

	template<typename T>
	void replace(const Any& ref)
	{
		delete data_;
		data_ = new TypicalHandler<T>(ref.as<T>());
	}

	template<typename T>
	void replace(const T& val)
	{
		delete data_;
		data_ = new TypicalHandler<T>(val);
	}

	template<typename T>
	T& as()
	{
		auto& w = dynamic_cast<TypicalHandler<std::decay_t<T>>&>(*data_);
		return *static_cast<std::decay_t<T>*>(w.data());
	}

	template<typename T>
	T const& as() const
	{
		auto const& w =
			dynamic_cast<TypicalHandler<std::decay_t<T>> const&>(*data_);
		return *static_cast<std::decay_t<T> const*>(w.data());
	}

	template<typename T>
	bool contains() const
	{
		return typeid(T) == data_->getType();
	}

private:
	Handler* data_;
};

template<typename T>
class Grid
{
public:
	Grid(size_t xSize, size_t ySize)
		: xSize_(xSize)
		, ySize_(ySize)
	{
		mem_ = new Any[xSize * ySize];
		for (size_t i = 0; i < xSize * ySize; i++) {
			mem_[i].replace((T)0);
		}
	}

	Grid(const Grid& ref)
		: xSize_(ref.xSize_)
		, ySize_(ref.ySize_)
	{
		mem_ = new Any[xSize_ * ySize_];
		for (size_t i = 0; i < xSize_ * ySize_; i++) {
			mem_[i].replace<T>(ref.mem_[i]);
		}
	}

	~Grid() { delete[] mem_; }

	Grid& operator=(const Grid& ref)
	{
		if (this == &ref) {
			return *this;
		}

		delete[] this->mem_;
		xSize_ = ref.getXSize();
		ySize_ = ref.getYSize();

		mem_ = new Any[xSize_ * ySize_];
		for (size_t i = 0; i < xSize_ * ySize_; i++) {
			mem_[i].replace<T>(ref.mem_[i]);
		}
		return *this;
	}

	T operator()(size_t xIdx, size_t yIdx) const
	{
		if (this->isSubgrid(xIdx, yIdx)) {
			return mem_[xIdx * ySize_ + yIdx].as<Grid<T>>().average_();
		}
		return mem_[xIdx * ySize_ + yIdx].as<T>();
	}

	T& operator()(size_t xIdx, size_t yIdx)
	{
		if (this->isSubgrid(xIdx, yIdx)) {
			return mem_[0].as<T>(); // UNDEFINED BEHAVIOUR
		}
		return mem_[xIdx * ySize_ + yIdx].as<T>();
	}

	size_t getXSize() const { return xSize_; }

	size_t getYSize() const { return ySize_; }

	Grid& operator=(T val)
	{
		for (size_t i = 0; i < xSize_ * ySize_; i++) {
			if (!this->isSubgrid(i / ySize_, i % ySize_)) {
				mem_[i].replace(val);
			}
			else {
				mem_[i].as<Grid<T>>() = val;
			}
		}
		return *this;
	}

	Grid& makeSubgrid(size_t xIdx,
		size_t yIdx,
		size_t xSubSize,
		size_t ySubSize)
	{
		auto oldVal = this->isSubgrid(xIdx, yIdx) ? (T)0 : (*this)(xIdx, yIdx);
		Grid<T> tmp(xSubSize, ySubSize);
		tmp = oldVal;
		mem_[xIdx * ySize_ + yIdx].replace<Grid<T>>(tmp);
		return *this;
	}

	Grid& collapseSubgrid(size_t xIdx, size_t yIdx)
	{
		if (!this->isSubgrid(xIdx, yIdx)) {
			return *this;
		}
		mem_[xIdx * ySize_ + yIdx].replace<T>(
			mem_[xIdx * ySize_ + yIdx].as<Grid<T>>().average_());
		return *this;
	}

	Grid& getSubgrid(size_t xIdx, size_t yIdx)
	{
		return mem_[xIdx * ySize_ + yIdx].as<Grid<T>>();
	}

	Grid const& getSubgrid(size_t xIdx, size_t yIdx) const
	{
		return mem_[xIdx * ySize_ + yIdx].as<Grid<T>>();
	}

	bool isSubgrid(size_t xIdx, size_t yIdx) const
	{
		return mem_[xIdx * ySize_ + yIdx].contains<Grid<T>>();
	}

private:
	T average_()
	{
		T sum = 0;
		for (size_t i = 0; i < xSize_ * ySize_; i++) {
			sum += (*this)(i / ySize_, i % ySize_);
		}
		return sum / (T)(xSize_ * ySize_);
	}

	Any* mem_;
	size_t xSize_, ySize_;
};

template<typename T>
std::istream&
operator>>(std::istream& f, Grid<T>& g)
{
	for (size_t i = 0; i < g.getXSize() * g.getYSize(); i++) {
		f >> g(i / g.getYSize(), i % g.getYSize());
	}
	return f;
}

template<typename T>
std::ostream&
operator<<(std::ostream& f, Grid<T> const& g)
{
	for (size_t i = 0; i < g.getXSize(); i++) {
		for (size_t j = 0; j < g.getYSize(); j++) {
			f << g(i, j) << '\t';
		}
		f << '\n';
	}
	f << '\n';
	return f;
}

int
main()
{
	Grid<double> frst(4, 3);
	std::cout << frst;
	frst = 8;
	frst(2, 0) = 123;
	std::cout << frst;

	std::cin >> frst;
	std::cout << frst;

	Grid<double> scnd = frst;
	std::cout << scnd(0, 1) << '\n';

	Grid<double> thrd(5, 5);
	thrd = 10;
	scnd = thrd;

	scnd.makeSubgrid(1, 1, 5, 5);

	std::cout << scnd.isSubgrid(1, 1) << '\n';

	scnd.makeSubgrid(1, 1, 5, 5);
	scnd.collapseSubgrid(1, 1);

	std::cout << scnd.isSubgrid(1, 1) << '\n';

	return 0;
}
