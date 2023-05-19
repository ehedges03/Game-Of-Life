#pragma once
#include "BitArray.h"

struct Varient {
	uint32_t ySide;
	uint32_t xSide;
	uint32_t xCorner;
	uint32_t yCorner;
	uint8_t offset;
};

class GameBoard {
public:
	GameBoard(uint32_t chuckWidth, uint32_t chunksPerRow);
	~GameBoard();

	void setPoint(uint32_t x, uint32_t y, bool value);
	bool getPoint(int x, int y);

	// TODO: returns the next status (alive/dead) of a point on the gameboard
	bool nextPointStatus(int x, int y);

private:
	class Chunk;
	friend class Chunk;

	const uint32_t c_sideLength;
	const uint32_t c_chunkWidth;
	const uint32_t c_chunksPerRow;
	BitArray m_points;
	Varient m_varients[4];
	Chunk*** m_chunks;

	// helper function used by nextPointStatus
	uint8_t countNeighbors(uint32_t x, uint32_t y);
	
	uint32_t xyToIndex(int x, int y);
};

class GameBoard::Chunk {

public:
	Chunk(GameBoard& gb, const Varient& varient, uint32_t offsetX, uint32_t offsetY);

	uint8_t countNeighbors(uint32_t x, uint32_t y);
	bool nextPointStatus(int x, int y);

	void processChunk();
	void writeBorder();

private:
	GameBoard& m_gb;
	BitArray m_border;
	const Varient& m_varient;
	const uint32_t c_offsetX;
	const uint32_t c_offsetY;
	bool m_processNextFrame = false;

	// Helper function to check and add a value to the buffer if it is in the border
	bool addedToBorderBuffer(int x, int y, bool value);
};