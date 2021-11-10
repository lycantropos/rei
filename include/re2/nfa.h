#ifndef RE2_NFA_H_
#define RE2_NFA_H_

#include <string.h>

#include <deque>

#include "re2/pod_array.h"
#include "re2/prog.h"
#include "re2/sparse_array.h"
#include "re2/stringpiece.h"

namespace re2 {
class NFA {
 public:
  NFA(const Prog* prog);
  ~NFA();

  // Searches for a matching string.
  //   * If anchored is true, only considers matches starting at offset.
  //     Otherwise finds lefmost match at or after offset.
  //   * If longest is true, returns the longest match starting
  //     at the chosen start point.  Otherwise returns the so-called
  //     left-biased match, the one traditional backtracking engines
  //     (like Perl and PCRE) find.
  // Records submatch boundaries in submatch[1..nsubmatch-1].
  // Submatch[0] is the entire match.  When there is a choice in
  // which text matches each subexpression, the submatch boundaries
  // are chosen to match what a backtracking implementation would choose.
  bool Search(const StringPiece& text, const StringPiece& context,
              bool anchored, bool longest, StringPiece* submatch,
              int nsubmatch);

 private:
  struct Thread {
    union {
      int ref;
      Thread* next;  // when on free list
    };
    const char** capture;
  };

  // State for explicit stack in AddToThreadq.
  struct AddState {
    int id;     // Inst to process
    Thread* t;  // if not null, set t0 = t before processing id
  };

  // Threadq is a list of threads.  The list is sorted by the order
  // in which Perl would explore that particular state -- the earlier
  // choices appear earlier in the list.
  typedef SparseArray<Thread*> Threadq;

  inline Thread* AllocThread();
  inline Thread* Incref(Thread* t);
  inline void Decref(Thread* t);

  // Follows all empty arrows from id0 and enqueues all the states reached.
  // Enqueues only the ByteRange instructions that match byte c.
  // context is used (with p) for evaluating empty-width specials.
  // p is the current input position, and t0 is the current thread.
  void AddToThreadq(Threadq* q, int id0, int c, const StringPiece& context,
                    const char* p, Thread* t0);

  // Run runq on byte c, appending new states to nextq.
  // Updates matched_ and match_ as new, better matches are found.
  // context is used (with p) for evaluating empty-width specials.
  // p is the position of byte c in the input string for AddToThreadq;
  // p-1 will be used when processing Match instructions.
  // Frees all the threads on runq.
  // If there is a shortcut to the end, returns that shortcut.
  int Step(Threadq* runq, Threadq* nextq, int c, const StringPiece& context,
           const char* p);

  // Returns text version of capture information, for debugging.
  std::string FormatCapture(const char** capture);

  void CopyCapture(const char** dst, const char** src) {
    memmove(dst, src, ncapture_ * sizeof src[0]);
  }

  const Prog* prog_;          // underlying program
  int start_;                 // start instruction in program
  int ncapture_;              // number of submatches to track
  bool longest_;              // whether searching for longest match
  bool endmatch_;             // whether match must end at text.end()
  const char* btext_;         // beginning of text (for FormatSubmatch)
  const char* etext_;         // end of text (for endmatch_)
  Threadq q0_, q1_;           // pre-allocated for Search.
  PODArray<AddState> stack_;  // pre-allocated for AddToThreadq
  std::deque<Thread> arena_;  // thread arena
  Thread* freelist_;          // thread freelist
  const char** match_;        // best match so far
  bool matched_;              // any match so far?

  NFA(const NFA&) = delete;
  NFA& operator=(const NFA&) = delete;
};
}  // namespace re2

#endif  // RE2_NFA_H_
