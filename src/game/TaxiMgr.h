/****************************************************************************
 *
 * Taxi System
 * Copyright (c) 2007 Antrix Team
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech ASA of Norway and appearing in the file
 * COPYING included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef __TAXIMGR_H
#define __TAXIMGR_H

#define TAXI_TRAVEL_SPEED 32

class Player;

struct TaxiNode {
	uint32 id;
	float x,y,z;
	uint32 mapid;
	uint32 horde_mount;
	uint32 alliance_mount;
};

struct TaxiPathNode 
{
	float x,y,z;
	uint32 mapid;
};

class TaxiPath {
	friend class TaxiMgr;

public:
	TaxiPath() 
	{ 
		m_length = 0;
	}

	~TaxiPath() 
	{
		while(m_pathNodes.size())
		{
			TaxiPathNode *pn = m_pathNodes.begin()->second;
			m_pathNodes.erase(m_pathNodes.begin());
			delete pn;
		}
	}

	inline const float & getLength( ) const { return m_length; };
	void ComputeLen();
	void SetPosForTime(float &x, float &y, float &z, uint32 time, uint32* lastnode);
	inline uint32 GetID() { return id; }
	void SendMoveForTime(Player *riding, Player *to, uint32 time);
	void AddPathNode(uint32 index, TaxiPathNode* pn) { m_pathNodes[index] = pn; }
	inline uint32 GetNodeCount() { return m_pathNodes.size(); }
	TaxiPathNode* GetPathNode(uint32 i);
	inline uint32 GetPrice() { return price; }

protected:

	std::map<uint32, TaxiPathNode*> m_pathNodes;

	float m_length;
	uint32 id, to, from, price;
};


class TaxiMgr :  public Singleton < TaxiMgr >
{
public:
	TaxiMgr() 
	{
		_LoadTaxiNodes();
		_LoadTaxiPaths();
	}

	~TaxiMgr() 
	{ 
		while(m_taxiPaths.size())
		{
			TaxiPath *p = m_taxiPaths.begin()->second;
			m_taxiPaths.erase(m_taxiPaths.begin());
			delete p;
		}
		while(m_taxiNodes.size())
		{
			TaxiNode *n = m_taxiNodes.begin()->second;
			m_taxiNodes.erase(m_taxiNodes.begin());
			delete n;
		}
	}

	TaxiPath *GetTaxiPath(uint32 path);
	TaxiPath *GetTaxiPath(uint32 from, uint32 to);
	TaxiNode *GetTaxiNode(uint32 node);

	uint32 GetNearestTaxiNode( float x, float y, float z, uint32 mapid );
	bool GetGlobalTaxiNodeMask( uint32 curloc, uint32 *Mask );


private:
	void _LoadTaxiNodes();
	void _LoadTaxiPaths();

	HM_NAMESPACE::hash_map<uint32, TaxiNode*> m_taxiNodes;
	HM_NAMESPACE::hash_map<uint32, TaxiPath*> m_taxiPaths;
};

#define sTaxiMgr TaxiMgr::getSingleton()

#endif
