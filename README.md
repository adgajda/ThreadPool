# ThreadPool

## Example

```cpp
#include "include/ThreadPool.hpp"
#include <future>
#include <iostream>

int main()
{
    ThreadPool thread_pool(2);
    std::future<int> result;

    result = thread_pool.add_task([] { return 3; });
    std::cout << result.get() << '\n';
    thread_pool.shutdown();

    return 0;
}
```

## Build and Run example

```
mkdir build && cd build
cmake ..
cmake --build .
./example
3
```
