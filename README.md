## Synopsis

A library to assist matrix allocation and perform cell-wise calculations between matrices and scalar values. 

## Code Example

The examples are based on 2D arrays, but up to 4D arrays are supported.

```cpp
// 2D Example
size_t height = 3, width = 2;
std::vector<size_t> dimensions = {height, width};

// Allocate 3x2 table of doubles
smartix<double> dbltbl(dimensions);
// Assign values in the cells
for (size_t r = 0; r < height; ++r)
  for (size_t c = 0; c < width; ++c) {
    dbltbl.uacc(r * width + c) = static_cast<double>((r * width + c) * 2.6);
  }
std::cout << "Table of doubles:" << std::endl;
printtable(&dbltbl.uacc(0), height, width);

// Create table of integers from table of doubles
smartix<int> inttbl = dbltbl;
std::cout << "Table of integers, copied for doubles:" << std::endl;
printtable(&inttbl.uacc(0), height, width);

// Multiply table of doubles with this of integers
dbltbl *= inttbl;
std::cout << "Table doubles *= table of integers:" << std::endl;
printtable(&dbltbl.uacc(0), height, width);

// Create table of floats from the sum of table of integers and doubles
smartix<float> flttbls = dbltbl + inttbl;
std::cout << "Table of floats = table of doubles + integers:" << std::endl;
printtable(&flttbls.uacc(0), height, width);

// Divide tables of doubles by a double value (2.0)
dbltbl = 3 + (flttbls / 2.0) * 3.2;
std::cout << "Table doubles 3 + (flttbls / 2.0) * 3.2:" << std::endl;
printtable(&dbltbl.uacc(0), height, width);
```

## License

MIT License

Copyright (c) 2017-2018 Christos Bontozoglou

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
