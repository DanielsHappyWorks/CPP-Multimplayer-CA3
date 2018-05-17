#include <PanzerPCH.h>

Pickup::Pickup()
{
	SetScale( GetScale() * 0.25f );
	SetCollisionRadius( 0.125f );
}


bool Pickup::HandleCollisionWithCat( Panzer* inCat )
{
	( void ) inCat;
	return false;
}



uint32_t Pickup::Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const 
{
	uint32_t writtenState = 0;

	if (inDirtyState & EMRS_Type)
	{
		inOutputStream.Write((bool)true);

		inOutputStream.Write(static_cast<int>(type));

		writtenState |= EMRS_Type;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if( inDirtyState & EMRS_Pose )
	{
		inOutputStream.Write( (bool)true );

		Vector3 location = GetLocation();
		inOutputStream.Write( location.mX );
		inOutputStream.Write( location.mY );

		inOutputStream.Write( GetRotation() );

		writtenState |= EMRS_Pose;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}

	if( inDirtyState & EMRS_Color )
	{
		inOutputStream.Write( (bool)true );

		inOutputStream.Write( GetColor() );

		writtenState |= EMRS_Color;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}


	return writtenState;
}

void Pickup::Read( InputMemoryBitStream& inInputStream )
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		int type;
		inInputStream.Read(type);
		setType(type);
	}

	inInputStream.Read( stateBit );
	if( stateBit )
	{
		Vector3 location;
		inInputStream.Read( location.mX );
		inInputStream.Read( location.mY );
		SetLocation( location );

		float rotation;
		inInputStream.Read( rotation );
		SetRotation( rotation );
	}


	inInputStream.Read( stateBit );
	if( stateBit )
	{	
		Vector3 color;
		inInputStream.Read( color );
		SetColor( color );
	}
}

