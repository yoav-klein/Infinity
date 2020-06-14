
#include "checkers.h"

namespace checkers
{

Piece::Piece(Position pos, Board& board, enum Color color)
	: m_pos(pos), m_board(board), m_color(color)
{
}
	
enum Color Piece::GetColor() const
{
	return m_color;
}
	
Position Piece::GetPosition() const
{
	return m_pos;
}

std::vector<std::tr1::shared_ptr<Move> > Piece::GetMoves() const
{
	return m_tourist->GetMoves();
}
	
void Piece::SetupPiece()
{
	m_board.SetPieceInPlace(this, m_pos);
	m_tourist.reset(new TouristRegular(*this, m_board));
}

void Piece::BecomeQueen()
{
	m_tourist.reset(new TouristQueen(*this, m_board));
}

void Piece::Die()
{
	m_board.RemovePiece(this);
}


bool Piece::IsQueen() const
{
	return m_tourist->IsQueen();
}
	
bool Piece::IsEating() const
{
	std::vector<std::tr1::shared_ptr<Move> > moves = GetMoves();
	for(std::size_t i = 0; i < moves.size(); ++i)
	{
		if(moves[i]->IsEating())
		{
			return true;
		}
	}
	return false;
}

void Piece::MovePiece(Position pos_new)
{
	m_board.MovePiece(this, pos_new);
	m_pos.SetPosition(pos_new.GetX(), pos_new.GetY());
}


} // namespace checkers
