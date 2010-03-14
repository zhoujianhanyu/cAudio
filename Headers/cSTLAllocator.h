#ifndef CSTLALLOCATOR_H_INCLUDED
#define CSTLALLOCATOR_H_INCLUDED

#include "../include/cAudioDefines.h"
#include "../Headers/cMemoryOverride.h"

namespace cAudio
{
	template<typename T>
	struct cSTLAllocatorBase
	{	// base class for generic allocators
		typedef T value_type;
	};

	template<typename T>
	struct cSTLAllocatorBase<const T>
	{	// base class for generic allocators for const T
		typedef T value_type;
	};

	template <typename T> class cSTLAllocator : public cSTLAllocatorBase<T>
	{
	public:
		typedef cSTLAllocatorBase<T>		Base;
		typedef typename Base::value_type	value_type;
		typedef value_type*					pointer;
		typedef const value_type*			const_pointer;
		typedef value_type&					reference;
		typedef const value_type&			const_reference;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;

		template<typename U>
		struct rebind
		{
			typedef cSTLAllocator<U> other;
		};

		cSTLAllocator()
		{ }

		~cSTLAllocator() throw()
		{ }

		cSTLAllocator( const cSTLAllocator& ) throw()
		{ }

		template <typename U>
		cSTLAllocator( const cSTLAllocator<U>& ) throw()
		{ }

		pointer address(reference x) const
		{
			return &x;
		}

		const_pointer address(const_reference x) const
		{
			return &x;
		}

		inline pointer allocate( size_type count, typename std::allocator<void>::const_pointer ptr = 0 )
		{
            (void)ptr;
			register size_type size = count*sizeof( T );
			pointer p  = static_cast<pointer>(CAUDIO_MALLOC(size));
			return p;
		}

		inline void deallocate( pointer p, size_type size )
		{
			CAUDIO_FREE(p);
		}

		size_type max_size() const throw()
		{
			return cAudio::getMemoryProvider()->getMaxAllocationSize();
		}

		void construct(pointer p, const T& val)
		{
			// call placement new
			new(static_cast<void*>(p)) T(val);
		}

		void destroy(pointer p)
		{
			p->~T();
		}
	};

	template<> class cSTLAllocator<void>
	{
    public:
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		typedef void*       pointer;
		typedef const void* const_pointer;
		typedef void        value_type;

		template<typename U>
		struct rebind
		{
			typedef cSTLAllocator<U> other;
		};
    };

	template <typename T>
	inline bool operator==(const cSTLAllocator<T>&, const cSTLAllocator<T>&)
	{
		return true;
	}

	template <typename T>
	inline bool operator!=(const cSTLAllocator<T>&, const cSTLAllocator<T>&)
	{
		return false;
	}

};

#endif //! CSTLALLOCATOR_H_INCLUDED