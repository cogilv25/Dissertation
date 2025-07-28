#include "AMaze.h"
#include "ProceduralMeshComponent.h"

void AMaze::BeginPlay()
{
	Super::BeginPlay();

	uint32 nStages = stages.Num();
	FString state = "";
	if (nStages > 0)
	{
		stages[0]->generate();
		state = stages[0]->state();
		for (uint32 i = 1; i < nStages; ++i)
		{
			stages[i]->axiom = state;
			stages[i]->generate();
			state = stages[i]->state();
			stages[i]->reset();
		}
	}

	runTurtle(state);

	for (int32 z  = 0; z < dimensions.Z; ++z)
	{
		for (int32 y = 0; y < dimensions.Y; ++y)
		{
			for (int32 x = 0; x < dimensions.X; ++x)
			{
				renderCell(x, y, z);
			}
		}
	}

	mesh->CreateMeshSection_LinearColor(0, vertices, indices, TArray<FVector>(), uv0, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true, false);
	mesh->SetMaterial(0, material);

	// Reset for next generation cycle.
	newMeshReady = false;
	generationComplete = false;
	turtleComplete = false;
	lSystemState = "";
	currentGenerationStage = 0;

	cells.Init(63, dimensions.X * dimensions.Y * dimensions.Z);
	turtleStringPosition = 0;
	stateStack.Empty();
	direction = FIntVector(1, 0, 0);
	upDir = FIntVector(0, 0, 1);
	position = FIntVector
	(
		startPos.X,
		startPos.Y,
		startPos.Z
	);

	vertices.Empty();
	indices.Empty();
	uv0.Empty();
	renderPosition = FIntVector(0, 0, 0);
}

void AMaze::renderCell(double x, double y, double z)
{
	uint8 cellValue = cells[x + (y * dimensions.X) + (z * dimensions.X * dimensions.Y)];
	
	if (!pipeMode)
	{
		// Render maze bounds.
		if (x == 0 && (cellValue & (uint8)1) == 1)
			renderCellWall(x - 1, y, z, 2);
		else if (x == dimensions.X - 1 && (cellValue & (uint8)2) == 2)
			renderCellWall(x + 1, y, z, 1);
		if (y == 0 && (cellValue & (uint8)4) == 4)
			renderCellWall(x, y - 1, z, 8);
		else if (y == dimensions.Y - 1 && (cellValue & (uint8)8) == 8)
			renderCellWall(x, y + 1, z, 4);
		if (z == 0 && (cellValue & (uint8)16) == 16)
			renderCellWall(x, y, z - 1, 32);
		else if (z == dimensions.Z - 1 && (cellValue & (uint8)32) == 32)
			renderCellWall(x, y, z + 1, 16);
	}

	// Filled cells do not need inward facing walls drawn
		//  as you cannot get into a position to see them.
	if (cellValue == 63)
		return;

	// Non-filled Cells
	if ((cellValue & (uint8)1) == 1)
		renderCellWall(x, y, z, 1);

	if ((cellValue & (uint8)2) == 2)
		renderCellWall(x, y, z, 2);

	if ((cellValue & (uint8)4) == 4)
		renderCellWall(x, y, z, 4);

	if ((cellValue & (uint8)8) == 8)
		renderCellWall(x, y, z, 8);

	if ((cellValue & (uint8)16) == 16)
		renderCellWall(x, y, z, 16);

	if ((cellValue & (uint8)32) == 32)
		renderCellWall(x, y, z, 32);

	// Outer "Pipes"
	if (pipeMode)
	{
		if ((cellValue & (uint8)1) == 1)
			renderCellWall(x - 1, y, z, 2);

		if ((cellValue & (uint8)2) == 2)
			renderCellWall(x + 1, y, z, 1);

		if ((cellValue & (uint8)4) == 4)
			renderCellWall(x, y - 1, z, 8);

		if ((cellValue & (uint8)8) == 8)
			renderCellWall(x, y + 1, z, 4);

		if ((cellValue & (uint8)16) == 16)
			renderCellWall(x, y, z - 1, 32);

		if ((cellValue & (uint8)32) == 32)
			renderCellWall(x, y, z + 1, 16);
	}
}

// index is the wrong word here... basically each bit represents an orientation
//    for a wall, this function only accepts an index where 1 bit is 1.
void AMaze::renderCellWall(double x, double y, double z, uint8 index)
{
	if (index == 0)
		return;

	double w = cellDimensions.X;
	double l = cellDimensions.Y;
	double h = cellDimensions.Z;

	// Model offset to move model to 0,0
	double mOX = w * dimensions.X / 2;
	double mOY = l * dimensions.Y / 2;
	double mOZ = h * dimensions.Z / 2;

	x = (x * w) - mOX;
	y = (y * l) - mOY;
	z = (z * h) - mOZ;

	int n = vertices.Num();

	//TODO: offset from centre -/+ cellWidth / 2???

	// -X Wall
	if (index == 1)
	{
		vertices.Push(FVector(x, y, z + h));
		vertices.Push(FVector(x, y + l, z + h));
		vertices.Push(FVector(x, y, z));
		vertices.Push(FVector(x, y + l, z));
	}
	// +X Wall
	else if (index == 2)
	{
		x += w;
		vertices.Push(FVector(x, y, z));
		vertices.Push(FVector(x, y + l, z));
		vertices.Push(FVector(x, y, z + h));
		vertices.Push(FVector(x, y + l, z + h));
	}
	// -Y Wall
	else if (index == 4)
	{
		vertices.Push(FVector(x, y, z));
		vertices.Push(FVector(x + w, y, z));
		vertices.Push(FVector(x, y, z + h));
		vertices.Push(FVector(x + w, y, z + h));
	}
	// +Y Wall
	else if (index == 8)
	{
		y += l;
		vertices.Push(FVector(x, y, z + h));
		vertices.Push(FVector(x + w, y, z + h));
		vertices.Push(FVector(x, y, z));
		vertices.Push(FVector(x + w, y, z));
	}
	// -Z Wall
	else if (index == 16)
	{
		vertices.Push(FVector(x, y, z));
		vertices.Push(FVector(x, y + l, z));
		vertices.Push(FVector(x + w, y, z));
		vertices.Push(FVector(x + w, y + l, z));
	}
	// +Z Wall
	else if (index == 32)
	{
		z += h;
		vertices.Push(FVector(x + w, y, z));
		vertices.Push(FVector(x + w, y + l, z));
		vertices.Push(FVector(x, y, z));
		vertices.Push(FVector(x, y + l, z));
	}
	else
		return;


	// 4 UV coords
	uv0.Push(FVector2D(0, 0));
	uv0.Push(FVector2D(0, 1));
	uv0.Push(FVector2D(1, 0));
	uv0.Push(FVector2D(1, 1));

	// 6 Indices
	indices.Push(n);
	indices.Push(n + 1);
	indices.Push(n + 2);
	indices.Push(n + 3);
	indices.Push(n + 2);
	indices.Push(n + 1);
}

void AMaze::turtleStep(FString::ElementType* CharPos)
{
	static FString::ElementType FChar = FString::ElementType(((int16)'F'));
	static FString::ElementType fChar = FString::ElementType(((int16)'f'));
	static FString::ElementType pushChar = FString::ElementType(((int16)'['));
	static FString::ElementType popChar = FString::ElementType(((int16)']'));
	static FString::ElementType pitchUChar = FString::ElementType(((int16)'^'));
	static FString::ElementType pitchDChar = FString::ElementType(((int16)'v'));
	static FString::ElementType rollRChar = FString::ElementType(((int16)'>'));
	static FString::ElementType rollLChar = FString::ElementType(((int16)'<'));
	static FString::ElementType yawRChar = FString::ElementType(((int16)'+'));
	static FString::ElementType yawLChar = FString::ElementType(((int16)'-'));


	if (*CharPos == pitchUChar)
	{
		FIntVector temp = FIntVector(-direction.X, -direction.Y, -direction.Z);
		direction = upDir;
		upDir = temp;
	}
	else if (*CharPos == pitchDChar)
	{
		FIntVector temp = FIntVector(-upDir.X, -upDir.Y, -upDir.Z);
		upDir = direction;
		direction = temp;
	}
	else if (*CharPos == rollLChar || *CharPos == rollRChar)
	{
		int32 rollDir = (*CharPos == rollLChar) ? 1 : -1;
		rollDir *= direction.X + direction.Y + direction.Z;
		if (direction.X != 0)
		{
			int z = upDir.Z, y = upDir.Y;
			upDir.Y = rollDir * (-z);
			upDir.Z = rollDir * y;
		}
		else if (direction.Y != 0)
		{
			int x = upDir.X, z = upDir.Z;
			upDir.Z = rollDir * (-x);
			upDir.X = rollDir * z;
		}
		else
		{
			int x = upDir.X, y = upDir.Y;
			upDir.Y = rollDir * (-x);
			upDir.X = rollDir * y;
		}
	}
	else if (*CharPos == yawLChar || *CharPos == yawRChar)
	{
		int32 yawDir = (*CharPos == yawLChar) ? 1 : -1;
		yawDir *= upDir.X + upDir.Y + upDir.Z;
		if (upDir.X != 0)
		{
			int z = direction.Z, y = direction.Y;
			direction.Z = yawDir * (-y);
			direction.Y = yawDir * z;
		}
		else if (upDir.Y != 0)
		{
			int x = direction.X, z = direction.Z;
			direction.X = yawDir * (-z);
			direction.Z = yawDir * x;
		}
		else
		{
			int x = direction.X, y = direction.Y;
			direction.Y = yawDir * (-x);
			direction.X = yawDir * y;
		}
	}
	else if (*CharPos == pushChar)
	{
		AmazeTurtleState state;

		state.x = position.X;
		state.y = position.Y;
		state.z = position.Z;

		state.dirX = direction.X;
		state.dirY = direction.Y;
		state.dirZ = direction.Z;

		state.upDX = upDir.X;
		state.upDY = upDir.Y;
		state.upDZ = upDir.Z;

		stateStack.Add(state);
	}
	else if (*CharPos == popChar)
	{
		auto state = stateStack[stateStack.Num() - 1];

		position.X = state.x;
		position.Y = state.y;
		position.Z = state.z;

		direction.X = state.dirX;
		direction.Y = state.dirY;
		direction.Z = state.dirZ;

		upDir.X = state.upDX;
		upDir.Y = state.upDY;
		upDir.Z = state.upDZ;

		stateStack.RemoveAt(stateStack.Num() - 1);
	}
	else if (*CharPos == fChar)
	{
		position.X += direction.X;
		position.Y += direction.Y;
		position.Z += direction.Z;
	}
	else if (*CharPos == FChar)
	{
		int32 oX = position.X, oY = position.Y, oZ = position.Z;
		position.X += direction.X;
		position.Y += direction.Y;
		position.Z += direction.Z;


		if (oX > -1 && oX < dimensions.X &&
			oY > -1 && oY < dimensions.Y &&
			oZ > -1 && oZ < dimensions.Z
			)
		{
			// This looks more complicated than it is.. it converts a number that could be
			//  -1, 0, 1 into 1, 0, 2 respectively then multiplies it by 1, 4 or 16 depending
			//  on the axis to get the bit for the wall we wish to remove if it exists.
			uint32 n = oX + (oY * dimensions.X) + (oZ * dimensions.X * dimensions.Y);
			uint8 wallBit =
				(uint8)(
					(direction.X * 2 + 3) % 3 +
					((direction.Y * 2 + 3) % 3) * 4 +
					((direction.Z * 2 + 3) % 3) * 16
					);
			// Now we remove it, if it exists by using bitwise and on the bitwise not and the
			//  cell we are working on therefore preserving all bits other than the one we wish
			//  to remove.
			cells[n] = cells[n] & (~wallBit);
		}

		// Do the same in the new cell but the wall in the opposite direction.
		if (position.X > -1 && position.X < dimensions.X &&
			position.Y > -1 && position.Y < dimensions.Y &&
			position.Z > -1 && position.Z < dimensions.Z
			)
		{
			uint32 n = position.X + (position.Y * dimensions.X) + (position.Z * dimensions.X * dimensions.Y);
			// -1, 0, 1 becomes 2, 0, 1 respectively this time flipping the direction.
			uint8 wallBit =
				(uint8)(
					(direction.X + 3) % 3 +
					((direction.Y + 3) % 3) * 4 +
					((direction.Z + 3) % 3) * 16
					);
			cells[n] = cells[n] & (~wallBit);
		}
	}
}

bool AMaze::runTurtle(int32 steps)
{
	if (turtleStringPosition >= lSystemState.Len()) return false;

	FString::ElementType* CharPos = ((FString::ElementType*)*lSystemState);
	CharPos += turtleStringPosition;
	int32 i = 0;

	for (;i + turtleStringPosition < lSystemState.Len() && i < steps; ++i)
	{
		turtleStep(CharPos);
		++CharPos;
	}

	turtleStringPosition += i;
	return turtleStringPosition >= lSystemState.Len();
}

void AMaze::runTurtle(FString in)
{
	cells.Init(63, dimensions.X * dimensions.Y * dimensions.Z);
	stateStack.Empty();

	direction = FIntVector( 1, 0, 0 );
	upDir = FIntVector( 0, 0, 1 );
	position = FIntVector(startPos.X, startPos.Y, startPos.Z);

	FString::ElementType* CharPos = (FString::ElementType*)*in;

	for (int32 i = 0; i < in.Len(); ++i)
	{
		turtleStep(CharPos);
		++CharPos;
	}
}

void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	generationTime += DeltaTime;
	// Update mesh the frame after it is ready.
	if (newMeshReady)
	{
		if (generationTime >= minimumGenerationTime)
		{
			// Swap mesh with new one.
			mesh->CreateMeshSection_LinearColor(0, vertices, indices, TArray<FVector>(), uv0, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true, false);
			mesh->SetMaterial(0, material);

			// Reset for next generation.
			newMeshReady = false;
			generationComplete = false;
			turtleComplete = false;
			lSystemState = "";
			currentGenerationStage = 0;

			cells.Init(63, dimensions.X * dimensions.Y * dimensions.Z);
			turtleStringPosition = 0;
			stateStack.Empty();
			direction = FIntVector(1, 0, 0);
			upDir = FIntVector(0, 0, 1);
			position = FIntVector(startPos.X, startPos.Y, startPos.Z);

			vertices.Empty();
			indices.Empty();
			uv0.Empty();
			renderPosition = FIntVector(0, 0, 0);

			UE_LOG(LogTemp, Warning, L"New Maze Generated in %f seconds", generationTime);
			generationTime = 0.0f;
		}
		return;
	}

	// Perform a fixed number of render steps per frame (Off-Screen).
	if (turtleComplete)
	{
		int32 i = 0;
		while (i < generationSpeed && renderPosition.Z < dimensions.Z)
		{
			renderCell(renderPosition.X, renderPosition.Y, renderPosition.Z);

			if (++renderPosition.X >= dimensions.X)
			{
				renderPosition.X = 0;
				if (++renderPosition.Y >= dimensions.Y)
				{
					renderPosition.Y = 0;
					if (++renderPosition.Z >= dimensions.Z) break;
				}
			}
			++i;
		}

		if (renderPosition.Z >= dimensions.Z)
			newMeshReady = true;

		return;
	}

	// Perform a fixed number of turtle steps per frame.
	if (generationComplete)
	{
		turtleComplete = runTurtle(generationSpeed);
		return;
	}

	if (stages.Num() < 1)
	{
		generationComplete = true;
		UE_LOG(LogTemp, Warning, L"No Stages To Generate");
		return;
	}

	// Perform a fixed number of generation steps per frame.
	if (stages[currentGenerationStage]->generate(generationSpeed))
	{
		lSystemState = stages[currentGenerationStage]->state();
		stages[currentGenerationStage]->reset();

		if (++currentGenerationStage == stages.Num())
			generationComplete = true;
		else
		{
			stages[currentGenerationStage]->axiom = lSystemState;
			stages[currentGenerationStage]->reset();
		}
	}
}

void AMaze::PostEditChangeProperty(FPropertyChangedEvent& pce)
{
	FString name = pce.Property->GetFName().ToString();

	if (name == "stages" && stages.Num() > 0)
	{
		for (int32 i = 0; i < stages.Num(); ++i)
		{
			if(stages[i] == nullptr)
				stages[i] = NewObject<USISLSystem>(this, USISLSystem::StaticClass());
			// This may be needed in a production (engine only) build.. I don't think so
			//  and at any rate I'm unlikely to make the aforementioned build but I'm unsure
			//  so I will leave it here in case I hit a confusing bug as a reminder
			// stages[i]->RegisterComponent();
			// stages[i]->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
	Super::PostEditChangeProperty(pce);
}

AMaze::AMaze()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>("mesh");
	mesh->SetupAttachment(GetRootComponent());

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
}