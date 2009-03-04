template<int,class,class> class Vector;
template<int Size, class Precision, int Stride, class Mem> struct GenericVBase;
template<typename T> class Operator;


////////////////////////////////////////////////////////////////////////////////
//
// Slice holding class
//

template<int Size, int Stride, class Precision> struct SliceVBase: public GenericVBase<Size, Precision, Stride, VectorSlice<Size, Precision> >{
	

	SliceVBase(Precision* d, int stride)
	:GenericVBase<Size, Precision, Stride, VectorSlice<Size, Precision> >(d, stride){
	}

	SliceVBase(Precision* d, int length, int stride)
	:GenericVBase<Size, Precision, Stride, VectorSlice<Size, Precision> >(d, length, stride){
	}

};

////////////////////////////////////////////////////////////////////////////////
//
// Classes for Vectors owning memory
//

// this class is really just a typedef
template<int Size, class Precision> struct VBase : public GenericVBase<Size, Precision, 1, VectorAlloc<Size, Precision> >{
	//Optional 
	
	VBase(){}

	VBase(int s)
	:GenericVBase<Size, Precision, 1, VectorAlloc<Size, Precision> >(s)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// Generic implementation
//

template<int Size, typename Precision, int Stride, typename Mem> struct GenericVBase: public Mem, public StrideHolder<Stride>
{	
	int stride() const{
		return StrideHolder<Stride>::stride();
	}

	//Optional constuctors
	GenericVBase(){}

	GenericVBase(int s)
	:Mem(s)
	{}

	GenericVBase(Precision* d, int stride)
	:Mem(d),StrideHolder<Stride>(stride){
	}

	GenericVBase(Precision* d, int length, int stride)
	:Mem(d, length),StrideHolder<Stride>(stride){
	}

	using Mem::my_data;
	using Mem::size;

	Precision& operator[](int i) {
		Internal::check_index(size(), i);
		return my_data[i * stride()];
	}

	const Precision& operator[](int i) const {
		Internal::check_index(size(), i);
		return my_data[i * stride()];
	}


	template<int Start, int Length> 
	Vector<Length, Precision, SliceVBase<Length, Stride, Precision> > slice(){
		Internal::CheckStaticSlice<Size, Start, Length>::check(size());
		return Vector<Length, Precision, SliceVBase<Length, Stride, Precision> >(my_data + stride()*Start, stride(), Slicing());
	}

	template<int Start, int Length> 
	const Vector<Length, Precision, SliceVBase<Length, Stride, Precision> > slice() const {
		Internal::CheckStaticSlice<Size, Start, Length>::check(size());
		return Vector<Length, Precision, SliceVBase<Length, Stride, Precision> >(const_cast<Precision*>(my_data + stride()*Start), stride(), Slicing());
	}

	Vector<-1, Precision, SliceVBase<-1, Stride, Precision> > slice(int start, int length){
		Internal::CheckDynamicSlice::check(size(), start, length);
		return Vector<-1, Precision, SliceVBase<-1, Stride, Precision> >(my_data + stride()*start, length, stride(), Slicing());
	}

	const Vector<-1, Precision, SliceVBase<-1, Stride, Precision> > slice(int start, int length) const {
		Internal::CheckDynamicSlice::check(size(), start, length);
		return Vector<-1, Precision, SliceVBase<-1, Stride, Precision> >(const_cast<Precision*>(my_data + stride()*start), length, stride(), Slicing());
	}

	const Matrix<1, Size, Precision, typename Slice<Stride>::ColMajor> as_row() const{
		return Matrix<1, Size, Precision, typename Slice<Stride>::ColMajor>(const_cast<Precision*>(my_data), 1, Size, stride(), Slicing());
	}

	Matrix<1, Size, Precision, typename Slice<Stride>::ColMajor> as_row(){
		return Matrix<1, Size, Precision, typename Slice<Stride>::ColMajor>(my_data, 1, Size, stride(), Slicing());
	}

	const Matrix<Size, 1, Precision, typename Slice<Stride>::RowMajor> as_col() const{
		return Matrix<Size, 1, Precision, typename Slice<Stride>::RowMajor>(const_cast<Precision*>(my_data), Size, 1, stride(), Slicing());
	}

	Matrix<Size, 1, Precision, typename Slice<Stride>::RowMajor> as_col(){
		return Matrix<Size, 1, Precision, typename Slice<Stride>::RowMajor>(my_data, Size, 1, stride(), Slicing());
	}
};
