{
	{	// Test onResize
		struct TestArray : public Array<int>{
			TestArray(): resized(false){}

			void onResize(){
				resized = true;
			}
			
			bool resized;
		};
	
		TestArray a;
		assert(!a.resized);	// default ctor should not resize

		a.resized = false;	// onResize should be called whenever the size changes
		a.resize(16);
		assert(a.resized);
		a.resized = false;	
		a.resize(8);
		assert(a.resized);
		
		a.resized = false;	// onResize should NOT be called since the size didn't change
		a.resize(8);
		assert(!a.resized);
	}

	{
		const int N=16;
		typedef int t;
		typedef Array<t> array_t;
		auto * a = new array_t(N, 123);
		auto * b = new array_t;
		b->source(*a);

		for(unsigned i=0; i<a->size(); ++i) assert((*a)[i] == 123);
		assert(a->data() == b->data());
		assert(a->size() == b->size());
		assert(array_t::references(a->data()) == 2);

		a->clear();
		assert(a->size() == 0);
		assert(a->data() == nullptr);
		assert(a->empty());

		delete a;
		assert(array_t::references(b->data()) == 1);

		{ // setting from raw pointer should not manage
			auto * c = new array_t(b->data(), b->size());
			assert(array_t::references(b->data()) == 1);
		}

		delete b;
		assert(array_t::references(b->data()) == 0);

		a = new array_t(N, 123);
		b = new array_t(*a);
		
		b->own();
		assert(a->data() != b->data());
		assert(array_t::references(a->data()) == 1);
		assert(array_t::references(b->data()) == 1);
		for(unsigned i=0; i<a->size(); ++i) assert((*b)[i] == 123);
		
		t * dataA = a->data();
		t * dataB = b->data();
		a->source(*b);
		assert(a->data() == b->data());
		assert(array_t::references(dataA) == 0);
		assert(array_t::references(dataB) == 2);
	}


	{
		DelayN<int> d(2);
	
		//for(int i=0; i<8; ++i) printf("%d\n", d(i+1));
	
		assert(d(1) == int());
		assert(d(2) == int());
		assert(d(3) == 1);
		assert(d(4) == 2);
		assert(d(5) == 3);
	}
	
//	{ Array<t> a(N); }
//	{ ArrayPow2<t> a(N); }
//	{ Ring<t> a(N); }
//	{ DoubleRing<t> a(N); a.copy(); }
}
