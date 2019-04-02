#ifndef __REPRESENTATIONS_HH__
#define __REPRESENTATIONS_HH__

#define likely(x) __builtin_expect(static_cast<bool>((x)), 1)
#define unlikely(x) __builtin_expect(static_cast<bool>((x)), 0)
#define iferr(condition) if (unlikely(condition))
#define ifsucc(condition) if (likely(condition))

#endif
