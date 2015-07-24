// AnalyzeType
// [-] Don't store an array of assignments; only true/false (definite) matters
// [-] Put block -> true/false assignment map on the slot, itself (and is_assigned(&block))
// [ ] Re-define non-local to mean crossing-scope (for current usage of non-
//     local just iter through and check at the end)
// [ ] Gather non-local (crossing-scope) assigns and uses
// [ ] Don't error on uninit, non-local use
// [ ] Loop (condition / block)
// [ ] And
// [ ] Or
