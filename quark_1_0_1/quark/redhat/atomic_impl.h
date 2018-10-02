// atomic_impl.h
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _REDHAT_ATOMIC_IMPL_H_
#define _REDHAT_ATOMIC_IMPL_H_

namespace quark
{

typedef struct { volatile int counter; } atomic_t; 

// atomic_inc - increment atomic variable
static inline void atomic_inc(atomic_t *v) 
{
	__asm__ __volatile__( "lock; incl %0"
						  :"=m" (v->counter)
						  :"m" (v->counter));
}

// atomic_dec - decrement atomic variable
static inline void atomic_dec(atomic_t *v) 
{

	__asm__ __volatile__( "lock; decl %0"
						  :"=m" (v->counter)
						  :"m" (v->counter));
}

// atomic_exchange - performs atomic exchange function 
static inline int atomic_exchange(volatile int * volatile obj, int reg)
{
	__asm__ __volatile__ ("xchgl %1,%0"
						  :"=r" (reg), "=m" (*obj)
						  :"r" (reg), "m" (*obj));
	return(reg);
}


} //namespace quark

#endif //_REDHAT_ATOMIC_IMPL_H_
