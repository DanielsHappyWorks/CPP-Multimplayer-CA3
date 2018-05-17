class PickupClient : public Pickup
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new PickupClient() ); }
	virtual void	Read(InputMemoryBitStream& inInputStream) override;

protected:
	PickupClient();

	void setType(int t);

private:

	SpriteComponentPtr	mSpriteComponent;
};