# IPv4 Extractor

Extracts one valid IPv4 address (with optional port) from each line of input, parsed by hand with no library conversion functions.

## Build and run

```
g++ -std=c++17 -Wall -Wextra -o ipv4 ipv4.cpp
./ipv4
```

## Run the tests

```
./ipv4 < tests.txt > actual_output.txt
diff actual_output.txt expected_output.txt
```

No output from `diff` means all 30 tests pass.

Note: since input comes from `tests.txt` instead of being typed, the input text doesn't appear in the output. Line N of `tests.txt` matches line N of `expected_output.txt`.

## Files

- `ipv4.cpp`: source code
- `tests.txt`: test inputs, one per line, ending with END
- `expected_output.txt`: expected program output for `tests.txt`
- `IPV4 Assignment_AI_Disclosure.docx`: AI usage disclosure
