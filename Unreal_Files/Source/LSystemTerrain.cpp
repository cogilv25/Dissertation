#include "LSystemTerrain.h"
#include "ProceduralMeshComponent.h"
#include "LSystem.h"


ALSystemTerrain::ALSystemTerrain()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>("mesh");
	mesh->SetupAttachment(GetRootComponent());

	generator = CreateDefaultSubobject<ULSystem>("generator");
}


void ALSystemTerrain::PostEditChangeProperty(struct FPropertyChangedEvent& pce)
{
	LSystemChanged();
	Super::PostEditChangeProperty(pce);
}

TArray<float> ALSystemTerrain::runTurtle(FString input)
{
	uint32 pmWidth = width + 1, pmHeight = height + 1;
	TArray<float> pm;
	pm.Init(0.0f, pmWidth * pmHeight);

	int32 directions[] = { 0,-1,0,1,0 }; // XDir = directions[turtleDir], YDir = directions[turtleDir + 1]
	uint32 turtleX = offsetX % pmWidth, turtleY = offsetY % pmHeight, turtleDir = 0;
	FString::ElementType uChar = FString::ElementType(((int16)'u'));
	FString::ElementType dChar = FString::ElementType(((int16)'d'));
	FString::ElementType fChar = FString::ElementType(((int16)'f'));
	FString::ElementType plusChar = FString::ElementType(((int16)'+'));
	FString::ElementType minusChar = FString::ElementType(((int16)'-'));

	uint32 nChars = input.Len();
	FString::ElementType* CharPos = (FString::ElementType*)*input;

	auto turtleStepForward = [&]()
		{
			turtleX = (turtleX + directions[turtleDir] + pmWidth) % pmWidth;
			turtleY = (turtleY + directions[turtleDir + 1] + pmHeight) % pmHeight;
		};

	for (uint32 i = 0; i < nChars; ++i)
	{
		if (*CharPos == plusChar)
			turtleDir = (turtleDir + 1) % 4;
		else if (*CharPos == minusChar)
			turtleDir = (turtleDir + 3) % 4;
		else if (*CharPos == uChar)
		{
			pm[turtleX + turtleY * pmWidth] += amplify;
			turtleStepForward();
		}
		else if (*CharPos == dChar)
		{
			pm[turtleX + turtleY * pmWidth] -= amplify;
			turtleStepForward();
		}
		else if (*CharPos == fChar)
				turtleStepForward();
		++CharPos;
	}
	return pm;
}

void ALSystemTerrain::LSystemChanged()
{
	mesh->ClearAllMeshSections();
	generator->reset();

	for (uint32 i = 0; i < generatorIterations; ++i)
		generator->stepForward();

	auto pointMap = runTurtle(generator->state());

	TArray<FVector> vertices;
	TArray<int> indices;
	TArray<FVector2D> uv0;

	// Shorthand to make things more readable
	double w = cellWidth;
	double h = cellHeight;
	// Model offset to move model to 0,0
	double mOX = w * width / 2;
	double mOY = h * height / 2;
	for (int64 y = 0; y < height; ++y)
	{
		for (int64 x = 0; x < width; ++x)
		{
			// a,b,c,d are the four corners on the point map for the current cell.
			uint64 a = x + y * (width + 1);
			uint64 b = a + width + 1;
			uint64 c = a + 1, d = b + 1;

			// 4 Vertices
			vertices.Push(FVector(x * w - mOX, y * h - mOY, pointMap[a]));
			vertices.Push(FVector(x * w - mOX, y * h + h - mOY, pointMap[b]));
			vertices.Push(FVector(x * w + w - mOX, y * h - mOY, pointMap[c]));
			vertices.Push(FVector(x * w + w - mOX, y * h + h - mOY, pointMap[d]));

			// 4 UV coords
			uv0.Push(FVector2D(0, 0));
			uv0.Push(FVector2D(0, 1));
			uv0.Push(FVector2D(1, 0));
			uv0.Push(FVector2D(1, 1));

			// 6 Indices
			int n = ((y * width) + x) * 4;
			indices.Push(n);
			indices.Push(n + 1);
			indices.Push(n + 2);
			indices.Push(n + 3);
			indices.Push(n + 2);
			indices.Push(n + 1);
		}
	}

	mesh->CreateMeshSection(0, vertices, indices, TArray<FVector>(), uv0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	mesh->SetMaterial(0, material);
}

// This is a hack to refresh the terrain in editor when the LSystem changes.
//    for some reason changes in sub-objects are not handled by ue.
void ALSystemTerrain::OnConstruction(const FTransform& trans)
{
	generator->setParentLSystemChangedFunction([](void* parent)
		{ ((ALSystemTerrain*)parent)->LSystemChanged(); }, this);
	Super::OnConstruction(trans);
}