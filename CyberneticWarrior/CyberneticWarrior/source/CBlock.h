#ifndef	BLOCK_H_
#define BLOCK_H_

#include "CBase.h"

enum{BLOCK_SOLID = 0, BLOCK_PARTIAL, BLOCK_MOVING, BLOCK_UNSTABLE, BLOCK_TRAP};
class CBlock : public CBase
{
private:
	
	int	m_nBlockType;
	tVector2D m_vStartingPosition;

public:

	CBlock(void);
	~CBlock(void);

	void Update(float fElapsedTime);
	void Render(void);

	RECT GetRect(void) const;

	bool CheckCollision(CBase* pBase);

	void SetBlockType(int nBlockType);
	int	GetBlockType(void);

	/*tVector2D	GetStartingPos(void);
	void	SetStartingPos(tVector2D vStartingPos);*/

};
#endif