# interlocked_hl requirements
=============

## Overview

`interlocked_hl` is a collection of interlocked high level routines.

### Exposed API

```c
#define INTERLOCKED_HL_RESULT_VALUES \
    INTERLOCKED_HL_OK, \
    INTERLOCKED_HL_ERROR, \
    INTERLOCKED_HL_CHANGED

MU_DEFINE_ENUM(INTERLOCKED_HL_RESULT, INTERLOCKED_HL_RESULT_VALUES);

typedef bool (*INTERLOCKED_COMPARE_EXCHANGE_64_IF)(LONG64 target, LONG64 exchange);

MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_Add64WithCeiling, LONG64 volatile*, Addend, LONG64, Ceiling, LONG64, Value, LONG64*, originalAddend)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);
MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_SetAndWake, LONG volatile*, address, LONG, value)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);
MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_WaitForValue, LONG volatile*, address, LONG, value, DWORD, milliseconds)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);
MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_CompareExchange64If, LONG64 volatile*, target, LONG64, exchange, INTERLOCKED_COMPARE_EXCHANGE_64_IF, compare, LONG64 *, original_target)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);

```

###  InterlockedHL_Add64WithCeiling

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_Add64WithCeiling, LONG64 volatile*, Addend, LONG64, Ceiling, LONG64, Value, LONG64*, originalAddend)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);
```

`InterlockedHL_Add64WithCeiling` computes the sum of `Addend` and `Value` updates `Addend` with it and writes in `originalAddend` the original value of `Addend`.
If `Addend` + `Value` would result in a undefined behavior or if the result would be greater than `Ceiling` 
then `InterlockedHL_Add64WithCeiling` fails and returns `INTERLOCKED_HL_ERROR`.

**SRS_INTERLOCKED_HL_02_001: [** If `Addend` is `NULL` then `InterlockedHL_Add64WithCeiling` shall fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_006: [** If `originalAddend` is `NULL` then `InterlockedHL_Add64WithCeiling` shall fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_002: [** If `Addend` + `Value` would underflow then `InterlockedHL_Add64WithCeiling` shall fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_003: [** If `Addend` + `Value` would overflow then `InterlockedHL_Add64WithCeiling` shall fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_004: [** If `Addend` + `Value` would be greater than `Ceiling` then `InterlockedHL_Add64WithCeiling` shall fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_005: [** Otherwise, `InterlockedHL_Add64WithCeiling` shall atomically write in `Addend` the sum of `Addend` and `Value`, succeed and return `INTERLOCKED_HL_OK`. **]**

**SRS_INTERLOCKED_HL_02_007: [** In all failure cases `InterlockedHL_Add64WithCeiling` shall not modify `Addend` or `originalAddend`. **]**

###  InterlockedHL_WaitForValue

```c
MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_WaitForValue, LONG volatile*, address, LONG, value, DWORD, milliseconds)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);
```

`InterlockedHL_WaitForValue` waits for the value at a given address to be equal to a target value.

**SRS_INTERLOCKED_HL_01_002: [** If `address` is NULL, `InterlockedHL_WaitForValue` shall fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_01_003: [** If the value at `address` is equal to `value`, `InterlockedHL_WaitForValue` shall return `INTERLOCKED_HL_OK`. **]**

**SRS_INTERLOCKED_HL_01_004: [** If the value at `address` is not equal to `value`, `InterlockedHL_WaitForValue` shall wait until the value at `address` changes in order to compare it again to `value` by using `WaitOnAddress`. **]**

**SRS_INTERLOCKED_HL_01_005: [** When waiting for the value at address to change, the `milliseconds` argument value shall be used as timeout. **]**

**SRS_INTERLOCKED_HL_01_007: [** When `WaitOnAddress` succeeds, the value at address shall be compared to the target value passed in `value` by using `InterlockedAdd`. **]**

**SRS_INTERLOCKED_HL_01_008: [** If the value at `address` does not match, `InterlockedHL_WaitForValue` shall issue another call to `WaitOnAddress`. **]**

**SRS_INTERLOCKED_HL_01_006: [** If `WaitOnAddress` fails, `InterlockedHL_WaitForValue` shall fail and return `INTERLOCKED_HL_ERROR`. **]**


### InterlockedHL_CompareExchange64If
```c
MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_CompareExchange64If, LONG64 volatile*, target, LONG64, exchange, INTERLOCKED_COMPARE_EXCHANGE_64_IF, compare, LONG64 *, original_target)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);
```

`InterlockedHL_CompareExchange64If` attempts to change `target` to `exchange` if `compare` that takes the value of `target` and `exchange` evaluates to `true`. `InterlockedHL_CompareExchange64If` will write `original_target` with the initial value of `target`.
If `target` changes while the function executes then `InterlockedHL_CompareExchange64If` returns `INTERLOCKED_HL_CHANGED` and the calling code can decide to retry/timeout/exponential backoff (for example).

**SRS_INTERLOCKED_HL_02_008: [** If `target` is `NULL` then `InterlockedHL_CompareExchange64If` shall return fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_009: [** If `compare` is `NULL` then `InterlockedHL_CompareExchange64If` shall return fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_010: [** If `original_target` is `NULL` then `InterlockedHL_CompareExchange64If` shall return fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_011: [** `InterlockedHL_CompareExchange64If` shall acquire the initial value of `target`. **]**

**SRS_INTERLOCKED_HL_02_012: [** If `compare`(`target`, `exchange`) returns `true` then `InterlockedHL_CompareExchange64If` shall exchange `target` with `exchange`. **]**

**SRS_INTERLOCKED_HL_02_013: [** If `target` changed meanwhile then `InterlockedHL_CompareExchange64If` shall return return `INTERLOCKED_HL_CHANGED` and shall not peform any exchange of values. **]**

**SRS_INTERLOCKED_HL_02_014: [** If `target` did not change meanwhile then `InterlockedHL_CompareExchange64If` shall return return `INTERLOCKED_HL_OK` and shall peform the exchange of values. **]**

**SRS_INTERLOCKED_HL_02_015: [** If `compare` returns `false` then  `InterlockedHL_CompareExchange64If` shall not perform any exchanges and return `INTERLOCKED_HL_OK`. **]**

**SRS_INTERLOCKED_HL_02_016: [** `original_target` shall be set to the original value of `target`. **]**

### InterlockedHL_SetAndWake
```c
MOCKABLE_FUNCTION_WITH_RETURNS(, INTERLOCKED_HL_RESULT, InterlockedHL_SetAndWake, LONG volatile*, address, LONG, value)(INTERLOCKED_HL_OK, INTERLOCKED_HL_ERROR);
```

`InterlockedHL_SetAndWake` set the value at `address` to `value` and signals the change of value in `address`. This can be commonly used with `InterlockedHL_WaitForValue` to signal a waiting thread.

**SRS_INTERLOCKED_HL_02_020: [** If `address` is `NULL` then `InterlockedHL_SetAndWake` shall fail and return `INTERLOCKED_HL_ERROR`. **]**

**SRS_INTERLOCKED_HL_02_017: [** `InterlockedHL_SetAndWake` shall set `address` to `value`. **]**

**SRS_INTERLOCKED_HL_02_018: [** `InterlockedHL_SetAndWake` shall call `WakeByAddressSingle`. **]**

**SRS_INTERLOCKED_HL_02_019: [** `InterlockedHL_SetAndWake` shall succeed and return `INTERLOCKED_HL_OK`. **]**