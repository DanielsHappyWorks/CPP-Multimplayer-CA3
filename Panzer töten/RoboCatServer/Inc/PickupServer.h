class PickupServer : public Pickup
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new PickupServer() ); }
	void HandleDying() override;
	virtual bool		HandleCollisionWithCat( Panzer* inCat ) override;
	
protected:
	PickupServer();

};