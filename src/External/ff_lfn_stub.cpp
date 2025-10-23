// ff_lfn_stub.cpp
extern "C" {

// Stub for ff_wtoupper (just returns the input)
unsigned ff_wtoupper(unsigned c) {
    return c;
}

// Stub for ff_convert (just returns the input pointer)
char* ff_convert(char* str, unsigned codepage) {
    (void)codepage;  // avoid unused variable warning
    return str;
}

} // extern "C"
