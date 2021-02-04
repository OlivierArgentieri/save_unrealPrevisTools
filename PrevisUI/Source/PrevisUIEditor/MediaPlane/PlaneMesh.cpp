#include "PlaneMesh.h"

// Create the initial mesh
void UPlaneMesh::CreatePlane(FVector2D _size)
{
	// List all the vertices
	TArray<FVector> _vertices;
	_vertices.Add(FVector(0, -(_size.X / 2), -(_size.Y / 2)));
	_vertices.Add(FVector(0, _size.X / 2, -(_size.Y / 2)));
	_vertices.Add(FVector(0, -(_size.X / 2), _size.Y / 2));
	_vertices.Add(FVector(0, _size.X / 2, _size.Y / 2));

	// Add the vertices in order to define triangles
	TArray<int32> _triangles;
	_triangles.Add(0);
	_triangles.Add(1);
	_triangles.Add(2);
	_triangles.Add(3);
	_triangles.Add(2);
	_triangles.Add(1);

	// Set the vertices's normals
	TArray<FVector> _normals;
	_normals.Add(FVector(1, 0, 0));
	_normals.Add(FVector(1, 0, 0));
	_normals.Add(FVector(1, 0, 0));
	_normals.Add(FVector(1, 0, 0));

	// Set the vertices's UVs
	TArray<FVector2D> _UV0;
	_UV0.Add(FVector2D(0, 0));
	_UV0.Add(FVector2D(1, 0));
	_UV0.Add(FVector2D(0, 1));
	_UV0.Add(FVector2D(1, 1));

	// Set the vertices's color
	TArray<FLinearColor> _vertexColor;
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));

	// Set the vertices's tangents
	TArray<FProcMeshTangent> _tangents;
	_tangents.Add(FProcMeshTangent(0, 1, 0));
	_tangents.Add(FProcMeshTangent(0, 1, 0));
	_tangents.Add(FProcMeshTangent(0, 1, 0));
	_tangents.Add(FProcMeshTangent(0, 1, 0));

	// Create the mesh
	this->CreateMeshSection_LinearColor(0, _vertices, _triangles, _normals, _UV0, _vertexColor, _tangents, false);
}

// Update the mesh (faster than CreatePlane)
void UPlaneMesh::UpdatePlane(FVector2D _size)
{
	// List all the vertices
	TArray<FVector> _vertices;
	_vertices.Add(FVector(0, -(_size.X / 2), -(_size.Y / 2)));
	_vertices.Add(FVector(0, _size.X / 2, -(_size.Y / 2)));
	_vertices.Add(FVector(0, -(_size.X / 2), _size.Y / 2));
	_vertices.Add(FVector(0, _size.X / 2, _size.Y / 2));

	// Set the vertices's normals
	TArray<FVector> _normals;
	_normals.Add(FVector(1, 0, 0));
	_normals.Add(FVector(1, 0, 0));
	_normals.Add(FVector(1, 0, 0));
	_normals.Add(FVector(1, 0, 0));

	// Set the vertices's UVs
	TArray<FVector2D> _UV0;
	_UV0.Add(FVector2D(0, 0));
	_UV0.Add(FVector2D(1, 0));
	_UV0.Add(FVector2D(0, 1));
	_UV0.Add(FVector2D(1, 1));

	// Set the vertices's color
	TArray<FLinearColor> _vertexColor;
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));
	_vertexColor.Add(FLinearColor(0.5, 0.5, 0.5, 1));

	// Set the vertices's tangents
	TArray<FProcMeshTangent> _tangents;
	_tangents.Add(FProcMeshTangent(0, 1, 0));
	_tangents.Add(FProcMeshTangent(0, 1, 0));
	_tangents.Add(FProcMeshTangent(0, 1, 0));
	_tangents.Add(FProcMeshTangent(0, 1, 0));

	// Create the mesh
	this->UpdateMeshSection_LinearColor(0, _vertices, _normals, _UV0, _vertexColor, _tangents);
}