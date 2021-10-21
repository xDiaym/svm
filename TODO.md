# TODO

## In progress
- [ ] Think before implement
- [ ] Build with docker
  - [ ] Add tests
  - [ ] Add docs build
  - [ ] Add project build
- [ ] Add GitHub pages pipeline
- [ ] Refactor
  - [ ] Rename objects
  - [ ] Find easier way to store type and cast
  - [ ] Rename methods/strutures
  - [x] Delegate object management to GC
  - [ ] GC should be struct with queue of round stat.
- [ ] Add `SVM_ASSERT` to `svm_malloc`

## Done
- [x] Add GC
- [x] Add recursion protection in GC(`svm_object_traverse`)
- [x] Make `RELEASE` has no effect with `NULL`
- [x] Add `SAFE_CAST` macro
