#ifndef __CHECKERS_H__
#define __CHECKERS_H__

#include <tr1/memory>
#include <iostream>
#include <vector>
#include <list>
#include <unistd.h>

#include "glut_utils.h"


#define ADDRESS(x) std::cout << (void*)(x) << std::endl;


namespace checkers
{
class Position;
class Board;
class Piece;
class Tile;
class Move;
class Direction;
class TouristInterface;
class BoardHolder;

enum Color { WHITE, BLACK, NONE };



class Drawer
{
public:
	Drawer(BoardHolder* board, int board_size)
		: m_board(board), m_board_size(board_size), m_radius((board_size / 16) - 10) { }
	
	void StartDrawer();
	void DrawBoard();
	void DrawWhiteTile(int from_i, int from_j, int to_i, int to_j);

	void DrawContent(Position pos, int from_i, int from_j, int to_i, int to_j);
	void DrawWhitePiece(int from_i, int from_j, int to_i, int to_j);
	void DrawWhiteQueen(int from_i, int from_j, int to_i, int to_j);
	void DrawBlackPiece(int from_i, int from_j, int to_i, int to_j);
	void DrawBlackQueen(int from_i, int from_j, int to_i, int to_j);
	
	void DrawSelected(int from_i, int from_j, int to_i, int to_j);
	void AnnounceWinner(enum Color winner);
	
private:
	BoardHolder* m_board;
	int m_board_size;
	int m_radius;
};



class Direction
{
public:
	Direction(int x, int y)
		: m_x(x), m_y(y) { }
	
	int GetX() const { return m_x; } 
	int GetY() const { return m_y; }
private:
	int m_x;
	int m_y;
};


class Position
{
public:
	Position(int x, int y)
		: m_x(x), m_y(y) { }
	
	void SetPosition(int x, int y)
	{
		m_x = x;
		m_y = y;
	}
	
	int GetX() const
	{
		return m_x;
	}
	
	int GetY() const
	{
		return m_y;
	}
	
	bool operator==(const Position& other) const 
	{
		return (other.m_x == m_x) && (other.m_y == m_y);
	}
	
	void operator+=(const Direction& direction)
	{
		m_x += direction.GetX();
		m_y += direction.GetY();
	}
	
	bool IsValidPosition() const
	{
		return ((m_x >= 0) && (m_y >= 0) && (m_x <= 7) && (m_y <= 7)); 
	}

private:	
	int m_x;
	int m_y;
};

class Tile
{
public:
	
	Tile()
		: m_piece(0), m_is_taken(false) { }
	
	void SetPiece(Piece* piece)
	{
		m_piece = piece;
		m_is_taken = true;
	}
	
	void TakePiece()
	{
		m_is_taken = false;
	}
	Piece* GetPiece() const
	{
		return m_piece;
	}
	bool IsTaken() const
	{
		return m_is_taken;
	}

private:
	Piece* m_piece;
	bool m_is_taken;
};


class Board
{
public:
	
	enum Status { PLAY, SELECTED };
	Board();
	enum Color Select(int x, int y);
	void InitBoard();
	
	void SwitchTurn();
	void SetStatus(enum Status status);
	void MovePiece(Piece*, Position where_to);
	enum Color TileContent(Position pos) const;
	void SetPieceInPlace(Piece* piece, Position where);
	
	void RemovePiece(Piece *piece);
	
	enum Status GetStatus() const;
	Piece* GetPieceInPosition(Position pos) const;
	Piece* GetSelectedPiece() const;
	
private:
	std::vector<Piece*> GetMoveablePieces();
	bool MoveOrPickAnotherPiece(Position pos);
	void DrawBoard();
	bool PickPiece(Position pos);
	
	// data
	Tile m_board[8][8];
	std::list<Piece> m_teams[2];
	
	enum Color m_turn;
	enum Status m_status;
	Piece* m_selected_piece;
};

class BoardHolder
{
public:
	BoardHolder() 
		: m_board_holder(new Board()) { }
	
	void NewBoard()
	{
		m_board_holder.reset(new Board());
	}
	
	Board& operator*()
	{
		return *m_board_holder;
	}
	
	Board* operator->()
	{
		return m_board_holder.get();
	}
	
private:
	std::tr1::shared_ptr<Board> m_board_holder;
};

class Game
{
public:
	Game(int board_size);
	
	void NewGame();
	enum Color SelectTile(int x, int y);
	void Play();
	void Draw();
	int GetBoardSize();

private:
	BoardHolder m_board_holder;
	Drawer m_drawer;
	int m_board_size;
};

class Piece
{
public:
	Piece(Position pos, Board& board, enum Color color);
	
	std::vector<std::tr1::shared_ptr<Move> > GetMoves() const;
	void SetupPiece();
	void Die();
	void BecomeQueen();
	void MovePiece(Position pos);
	
	Position GetPosition() const;
	bool IsQueen() const;
	bool IsEating() const;
	enum Color GetColor() const;
	
private:
	Position m_pos;
	Board& m_board;
	std::tr1::shared_ptr<TouristInterface> m_tourist;
	enum Color m_color;
};


class Move
{
public:
	Move(Board& board, Piece& piece, Position where_to);
	
	Position WhereTo();
	virtual void Activate();
	virtual bool IsEating();
	
protected:
	Board& m_board;
	Piece& m_piece;
	Position m_where_to;
};

class Eat : public Move
{
public:
	Eat(Board& board, Piece& piece, Position where_to, Piece& who);
	
	Piece& EatingWho();
	virtual void Activate();
	virtual bool IsEating();
private:
	Piece& m_who;
};

class TouristInterface
{
public:

	TouristInterface(Piece& piece, Board& board)
		: m_piece(piece), m_board(board) 
	{
		
		m_directions.push_back(Direction(-1, 1));
		m_directions.push_back(Direction(-1, -1)); 
		m_directions.push_back(Direction(1, 1)); 
		m_directions.push_back(Direction(1, -1));
	}	
	
	virtual ~TouristInterface() { }	
	virtual std::vector<std::tr1::shared_ptr<Move> > GetMoves() = 0;
	virtual bool IsQueen() const = 0;
	
protected:
	Piece& m_piece;
	Board& m_board;
	std::vector<Direction> m_directions;
};


class TouristRegular : public TouristInterface 
{
public:
	TouristRegular(Piece& piece, Board& board)
		: TouristInterface(piece, board) { } 
	virtual ~TouristRegular() { }
	
	virtual std::vector<std::tr1::shared_ptr<Move> > GetMoves();
	virtual bool IsQueen() const;
	void TourDirection(Direction direction);
};

class TouristQueen : public TouristInterface
{
public:
	TouristQueen(Piece& piece, Board& board)
		: TouristInterface(piece, board) { } 
	virtual ~TouristQueen() { }	
	
	virtual bool IsQueen() const;	
	virtual std::vector<std::tr1::shared_ptr<Move> > GetMoves();
	void TourDirection(Direction direction, Position curr_pos);
};

struct IsSamePredicator
{
public:
	IsSamePredicator(Piece* piece)
		: m_piece(piece) { }
	
	bool operator()(const Piece& piece) { return m_piece == &piece; }

private:
	Piece* m_piece;
};

} // namespace checkers

#endif// __CHECKERS_H__ 


