#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(theseTestsName memory_data_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
    )

set(${theseTestsName}_c_files
../../src/memory_data.c
)

set(${theseTestsName}_h_files
)

build_c_tests(${theseTestsName} ON "tests/azure_c_util_tests" ADDITIONAL_LIBS azure_c_util)
