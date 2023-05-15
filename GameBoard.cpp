#include "pch.h"
#include "GameBoard.h"
#include <iostream>

GameBoard::GameBoard(uint32_t chunkSideSize, uint32_t chunksPerRow) :
	m_points(BitArray(chunkSideSize* chunkSideSize* chunksPerRow* chunksPerRow)),
	c_sideLength(chunkSideSize * chunksPerRow), c_chunkSideSize(chunkSideSize),
	c_chunksPerRow(chunksPerRow) {
	uint32_t currX = 0;
	uint32_t currY = 0;
	m_chunks = (Chunk***) new Chunk*[chunksPerRow * chunksPerRow];
	for (int i = 0; i < chunksPerRow; i++) {
		for (int j = 0; j < chunksPerRow; j++) {
			m_chunks[i][j] = new Chunk(*this, currX, currY);
			currX += chunkSideSize;
		}
		currX = 0;
		currY += chunkSideSize;
	}
}

void GameBoard::setPoint(uint32_t x, uint32_t y, bool value) {
	m_points.set(xyToIndex(x, y), value);
}

bool GameBoard::getPoint(uint32_t x, uint32_t y) {
	return m_points.get(xyToIndex(x, y));
}

uint32_t GameBoard::xyToIndex(uint32_t x, uint32_t y) {
	// All of these while loops could be if to avoid the double check if we assume
	// that a given x, y will not be further than one gameboard away (resonable assumption).
	while (x < 0) {
		x += c_sideLength;
	}
	while (x >= c_sideLength) {
		x -= c_sideLength;
	}
	while (y < 0) {
		y += c_sideLength;
	}
	while (y >= c_sideLength) {
		y -= c_sideLength;
	}

	return x + y * c_sideLength;
}

GameBoard::Chunk::Chunk(GameBoard& gb, uint32_t offsetX, uint32_t offsetY) :
	m_gb(gb), c_offsetX(offsetX), c_offsetY(offsetY) {
	
}

uint8_t GameBoard::Chunk::countNeighbors(uint32_t x, uint32_t y) {
	// Would it be faster or slower to check if the value is 4 to avoid checking all pixels
	// The if statements could lead to a slower processing time
	uint8_t count = 0;
	uint32_t topLeft = m_gb.xyToIndex(x - 1, y - 1);
	count += m_gb.m_points.get(topLeft);
	count += m_gb.m_points.get(topLeft + 1);
	count += m_gb.m_points.get(topLeft + 2);
	topLeft += m_gb.c_sideLength;
	count += m_gb.m_points.get(topLeft);
	count += m_gb.m_points.get(topLeft + 2);
	topLeft += m_gb.c_sideLength;
	count += m_gb.m_points.get(topLeft);
	count += m_gb.m_points.get(topLeft + 1);
	return count + m_gb.m_points.get(topLeft + 2);
}