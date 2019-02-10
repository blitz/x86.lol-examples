// An example program that XORs standard input with a value given on the command
// line and writes the result to the standard output.

#include <iostream>
#include <functional>
#include <vector>
#include <cstdlib>
#include <string>
#include <utility>

using xor_fn = int (*)(int);

template <int KEY>
int do_static_xor(int input)
{
  return input ^ KEY;
}

template<std::size_t... I>
static auto init_vector(std::index_sequence<I...>)
{
  std::initializer_list<xor_fn> init = { &do_static_xor<I>... };
  return std::vector<xor_fn>(init);
}

// Do generate lots of XOR instructions with immediate values, we generate a
// table of xor functions for values from 0 to 511.
static std::vector<xor_fn> optimized_fns = init_vector(std::make_index_sequence<512>());

int main(int argc, char **argv)
{
  if (argc != 2)
    return EXIT_FAILURE;

  int key = std::stoi(argv[1], nullptr, 0);

  xor_fn do_xor = key < optimized_fns.size() ? optimized_fns[key] : nullptr;

  union {
    int int_buf;
    char raw[sizeof(int)];
  };

  int buf;

  while (std::cin) {
    std::cin.read(raw, sizeof(raw));

    if (std::cin.gcount() != sizeof(raw))
      break;

    if (do_xor)
      int_buf = do_xor(int_buf);
    else
      int_buf = int_buf ^ key;

    std::cout.write(raw, sizeof(raw));
  }

  return EXIT_SUCCESS;
}
