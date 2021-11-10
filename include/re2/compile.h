#ifndef RE2_COMPILE_H_
#define RE2_COMPILE_H_

// Copyright 2007 The RE2 Authors.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Compile regular expression to Prog.
//
// Prog and Inst are defined in prog.h.
// This file's external interface is just Regexp::CompileToProg.
// The Compiler class defined in this file is private.

#include <unordered_map>

#include "re2/pod_array.h"
#include "re2/prog.h"
#include "re2/re2.h"
#include "re2/regexp.h"
#include "re2/walker-inl.h"

namespace re2 {

// List of pointers to Inst* that need to be filled in (patched).
// Because the Inst* haven't been filled in yet,
// we can use the Inst* word to hold the list's "next" pointer.
// It's kind of sleazy, but it works well in practice.
// See http://swtch.com/~rsc/regexp/regexp1.html for inspiration.
//
// Because the out and out1 fields in Inst are no longer pointers,
// we can't use pointers directly here either.  Instead, head refers
// to inst_[head>>1].out (head&1 == 0) or inst_[head>>1].out1 (head&1 == 1).
// head == 0 represents the NULL list.  This is okay because instruction #0
// is always the fail instruction, which never appears on a list.
struct PatchList {
  // Returns patch list containing just p.
  static PatchList Mk(uint32_t p) { return {p, p}; }

  // Patches all the entries on l to have value p.
  // Caller must not ever use patch list again.
  static void Patch(Prog::Inst* inst0, PatchList l, uint32_t p) {
    while (l.head != 0) {
      Prog::Inst* ip = &inst0[l.head >> 1];
      if (l.head & 1) {
        l.head = ip->out1();
        ip->out1_ = p;
      } else {
        l.head = ip->out();
        ip->set_out(p);
      }
    }
  }

  // Appends two patch lists and returns result.
  static PatchList Append(Prog::Inst* inst0, PatchList l1, PatchList l2) {
    if (l1.head == 0) return l2;
    if (l2.head == 0) return l1;
    Prog::Inst* ip = &inst0[l1.tail >> 1];
    if (l1.tail & 1)
      ip->out1_ = l2.head;
    else
      ip->set_out(l2.head);
    return {l1.head, l2.tail};
  }

  uint32_t head;
  uint32_t tail;  // for constant-time append
};

static const PatchList kNullPatchList = {0, 0};

// Compiled program fragment.
struct Frag {
  uint32_t begin;
  PatchList end;

  Frag() : begin(0) { end.head = 0; }  // needed so Frag can go in vector
  Frag(uint32_t begin, PatchList end) : begin(begin), end(end) {}
};

// Input encodings.
enum Encoding {
  kEncodingUTF8 = 1,  // UTF-8 (0-10FFFF)
  kEncodingLatin1,    // Latin-1 (0-FF)
};

class Compiler : public Regexp::Walker<Frag> {
 public:
  explicit Compiler();
  ~Compiler();

  // Compiles Regexp to a new Prog.
  // Caller is responsible for deleting Prog when finished with it.
  // If reversed is true, compiles for walking over the input
  // string backward (reverses all concatenations).
  static Prog* Compile(Regexp* re, bool reversed, int64_t max_mem);

  // Compiles alternation of all the re to a new Prog.
  // Each re has a match with an id equal to its index in the vector.
  static Prog* CompileSet(Regexp* re, RE2::Anchor anchor, int64_t max_mem);

  // Interface for Regexp::Walker, which helps traverse the Regexp.
  // The walk is purely post-recursive: given the machines for the
  // children, PostVisit combines them to create the machine for
  // the current node.  The child_args are Frags.
  // The Compiler traverses the Regexp parse tree, visiting
  // each node in depth-first order.  It invokes PreVisit before
  // visiting the node's children and PostVisit after visiting
  // the children.
  Frag PreVisit(Regexp* re, Frag parent_arg, bool* stop);
  Frag PostVisit(Regexp* re, Frag parent_arg, Frag pre_arg, Frag* child_args,
                 int nchild_args);
  Frag ShortVisit(Regexp* re, Frag parent_arg);
  Frag Copy(Frag arg);

  // Given fragment a, returns a+ or a+?; a* or a*?; a? or a??
  Frag Plus(Frag a, bool nongreedy);
  Frag Star(Frag a, bool nongreedy);
  Frag Quest(Frag a, bool nongreedy);

  // Given fragment a, returns (a) capturing as \n.
  Frag Capture(Frag a, int n);

  // Given fragments a and b, returns ab; a|b
  Frag Cat(Frag a, Frag b);
  Frag Alt(Frag a, Frag b);

  // Returns a fragment that can't match anything.
  Frag NoMatch();

  // Returns a fragment that matches the empty string.
  Frag Match(int32_t id);

  // Returns a no-op fragment.
  Frag Nop();

  // Returns a fragment matching the byte range lo-hi.
  Frag ByteRange(int lo, int hi, bool foldcase);

  // Returns a fragment matching an empty-width special op.
  Frag EmptyWidth(EmptyOp op);

  // Adds n instructions to the program.
  // Returns the index of the first one.
  // Returns -1 if no more instructions are available.
  int AllocInst(int n);

  // Rune range compiler.

  // Begins a new alternation.
  void BeginRange();

  // Adds a fragment matching the rune range lo-hi.
  void AddRuneRange(Rune lo, Rune hi, bool foldcase);
  void AddRuneRangeLatin1(Rune lo, Rune hi, bool foldcase);
  void AddRuneRangeUTF8(Rune lo, Rune hi, bool foldcase);
  void Add_80_10ffff();

  // New suffix that matches the byte range lo-hi, then goes to next.
  int UncachedRuneByteSuffix(uint8_t lo, uint8_t hi, bool foldcase, int next);
  int CachedRuneByteSuffix(uint8_t lo, uint8_t hi, bool foldcase, int next);

  // Returns true iff the suffix is cached.
  bool IsCachedRuneByteSuffix(int id);

  // Adds a suffix to alternation.
  void AddSuffix(int id);

  // Adds a suffix to the trie starting from the given root node.
  // Returns zero iff allocating an instruction fails. Otherwise, returns
  // the current root node, which might be different from what was given.
  int AddSuffixRecursive(int root, int id);

  // Finds the trie node for the given suffix. Returns a Frag in order to
  // distinguish between pointing at the root node directly (end.head == 0)
  // and pointing at an Alt's out1 or out (end.head&1 == 1 or 0, respectively).
  Frag FindByteRange(int root, int id);

  // Compares two ByteRanges and returns true iff they are equal.
  bool ByteRangeEqual(int id1, int id2);

  // Returns the alternation of all the added suffixes.
  Frag EndRange();

  // Single rune.
  Frag Literal(Rune r, bool foldcase);

  void Setup(Regexp::ParseFlags flags, int64_t max_mem, RE2::Anchor anchor);
  Prog* Finish(Regexp* re);

  // Returns .* where dot = any byte
  Frag DotStar();

 private:
  Prog* prog_;         // Program being built.
  bool failed_;        // Did we give up compiling?
  Encoding encoding_;  // Input encoding
  bool reversed_;      // Should program run backward over text?

  PODArray<Prog::Inst> inst_;
  int ninst_;      // Number of instructions used.
  int max_ninst_;  // Maximum number of instructions.

  int64_t max_mem_;  // Total memory budget.

  std::unordered_map<uint64_t, int> rune_cache_;
  Frag rune_range_;

  RE2::Anchor anchor_;  // anchor mode for RE2::Set

  Compiler(const Compiler&) = delete;
  Compiler& operator=(const Compiler&) = delete;
};

}  // namespace re2

#endif  // RE2_BITMAP256_H_
