The point of this project is to be able to run Texas Hold 'Em poker simulations as fast as humanly possible.

Design principles for performance:
1) Allocate nothing (or as little as possible, use fixed size arrays with value types that can be reused).
2) Debug asserts for error handling that can be disabled in release mode, once program is stable.
3) Keep data types small and avoid making copies.
4) Inline critical code executed in loops.
