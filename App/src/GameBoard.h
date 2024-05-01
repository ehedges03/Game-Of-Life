#pragma once
#include <array>
#include <bitset>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <utility>

class PairHash {
public:
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    return (53 + h1) * 53 + h2;
  }
};

class PairEqual {
public:
  template <class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    return (53 + h1) * 53 + h2;
  }
};

// Function template to calculate the number of set bits in a byte at compile
// time
constexpr int countSetBits(unsigned char byte) {
  int count = 0;
  for (int i = 0; i < 8; ++i) {
    count += (byte >> i) & 1;
  }
  return count;
}

// Template metafunction to generate the lookup table at compile time
template <int... Indices>
constexpr auto makeLookupTable(std::integer_sequence<int, Indices...>) {
  return std::array<int, sizeof...(Indices)>{countSetBits(Indices)...};
}

/**
 * 64 x 64 block of bits for gameboard.
 */
class Chunk;

/**
 * Main gameboard structure for working with chunks and controlling the system.
 */
class GameBoard {
public:
  GameBoard();
  void setPoint(int32_t x, int32_t y, bool value);
  bool getPoint(int32_t x, int32_t y);

  void update();

  friend std::ostream &operator<<(std::ostream &o, GameBoard &g);

private:
  static std::bitset<512> bitsToState;
  static bool bitsToStateSetup;
  friend class Chunk;

  std::unordered_map<std::pair<int32_t, int32_t>, Chunk, PairHash> m_chunks;

  int32_t m_maxX = 0;
  int32_t m_minX = 0;
  int32_t m_maxY = 0;
  int32_t m_minY = 0;

  std::optional<std::reference_wrapper<Chunk>> getChunk(int32_t x, int32_t y);
  Chunk &getOrMakeChunk(int32_t x, int32_t y);
};


class Chunk {

public:
  static constexpr int32_t Size = 64;

  struct Border {
    bool topLeft;
    bool topRight;
    bool bottomLeft;
    bool bottomRight;
    std::bitset<Size> top;
    std::bitset<Size> bottom;
    std::bitset<Size> left;
    std::bitset<Size> right;
  };

  Border border;

  void processNextState();
  void swapToNextState();
  bool empty();

  using iterator = std::array<std::bitset<Size>, Size>::iterator;
  using const_iterator = std::array<std::bitset<Size>, Size>::const_iterator;

  std::bitset<Size> &operator[](int32_t i);
  const std::bitset<Size> &operator[](int32_t i) const;

  iterator begin() { return m_dataBuffer[m_currBuffer].begin(); };
  const_iterator begin() const { return m_dataBuffer[m_currBuffer].begin(); };

  iterator end() { return m_dataBuffer[m_currBuffer].end(); };
  const_iterator end() const { return m_dataBuffer[m_currBuffer].end(); };

  friend std::ostream &operator<<(std::ostream &o, Chunk c);

private:
  std::array<std::array<std::bitset<Size>, Size>, 2> m_dataBuffer;
  uint32_t m_currBuffer = 0;
};
