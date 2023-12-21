#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>

inline std::size_t row_column_to_index(std::size_t row, std::size_t column, std::size_t size)
{
  return row * size + column;
}

inline std::size_t index_to_column(std::size_t index, std::size_t size)
{
  return index % size;
}

inline std::size_t index_to_row(std::size_t index, std::size_t size)
{
  // This is integer division, so no floating point result.
  return index / size;
}

inline void zero(float* matrix, std::size_t size)
{
  for (std::size_t i = 0; i < size; ++i) {
    for (std::size_t j = 0; j < size; ++j) {
      matrix[row_column_to_index(i, j, size)] = 0;
    }
  }
}

inline void eye(float* matrix, std::size_t size)
{
  zero(matrix, size);
  for (std::size_t i = 0; i < size; ++i) {
    matrix[row_column_to_index(i, i, size)] = 0;
  }
}

inline std::mt19937_64 generate_mersenne_twister()
{
  std::random_device rd;
  std::size_t nsize = 624;

  std::vector<std::uint32_t> seed(nsize);

  for (std::size_t i = 0; i < nsize; ++i) {
    seed[i] = rd();
  }

  std::seed_seq seed_seq(std::begin(seed), std::end(seed));

  return std::mt19937_64(seed_seq);
}

inline void random_matrix(float* matrix, std::size_t size, float min, float max)
{
  std::mt19937_64 prng = generate_mersenne_twister();

  std::uniform_real_distribution<float> urd(min, max);

  for (std::size_t i = 0; i < size; ++i) {
    for (std::size_t j = 0; j < size; ++j) {
      matrix[row_column_to_index(i, j, size)] = urd(prng);
    }
  }
}

inline void print_matrix(std::string name, float* matrix, std::size_t size)
{
  printf("%s = ", name.c_str());
  std::printf("[");
  for (std::size_t row = 0; row < size; ++row) {
    if (row != 0) {
      std::printf(",");
    }
    std::printf("[");
    for (std::size_t col = 0; col < size; ++col) {
      if (row != 0) {
        std::printf(",");
      }
      std::printf("%lf", matrix[row_column_to_index(row, col, size)]);
    }
    std::printf("]\n");
  }
  std::printf("]\n\n");
}
