class Pickup : public GameObject
{
public:
	CLASS_IDENTIFICATION( 'MOUS', GameObject )

	enum EPickupReplicationState
	{
		EMRS_Pose		= 1 << 0,
		EMRS_Color		= 1 << 1,
		EMRS_Type		= 1 << 2,

		EMRS_AllState	= EMRS_Pose | EMRS_Color | EMRS_Type
	};

	enum PickupType
	{
		health = 1,
		mine = 2
	};

	static	GameObject*	StaticCreate() { return new Pickup(); }

	virtual uint32_t	GetAllStateMask()	const override	{ return EMRS_AllState; }

	virtual uint32_t	Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const override;
	virtual void		Read( InputMemoryBitStream& inInputStream ) override;

	virtual bool HandleCollisionWithCat( Panzer* inCat ) override;
	void setType(int t) { type = static_cast<PickupType>(t);}
protected:
	Pickup();
	PickupType type;
};