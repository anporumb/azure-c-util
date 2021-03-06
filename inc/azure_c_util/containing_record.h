// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef CONTAINING_RECORD_H
#define CONTAINING_RECORD_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

//
// Calculate the address of the base of the structure given its type, and an
// address of a field within the structure.
//
#define containingRecord(address, type, field) ((type *)((uintptr_t)(address) - offsetof(type,field)))

#endif /* CONTAINING_RECORD_H */
