/* 
*  Kevin Meergans, SquadAI, 2014
*  Helpers.h
*  Contains static helper functions.
*/

#ifndef HELPERS_H
#define HELPERS_H

// Includes
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Computes the intersection of a ray derived from the passed in cursor position with the passed in plane and 
// stores the result in the intersection parameter.
// The code is mostly based on: http://www.mvps.org/directx/articles/rayproj.htm
//--------------------------------------------------------------------------------------
static void GetIntersection( const POINT& cursorPosition, const XMFLOAT4* const pPlane, int windowWidth, int windowHeight, const XMFLOAT4X4* const pViewMatrix, XMFLOAT3* pIntersection )
{
	// This code has to be tested yet

	// Transform to ray in view space

	XMFLOAT3 pickingRay;
	pickingRay.x = (cursorPosition.x / (windowWidth * 0.5f) - 1.0f);
	pickingRay.y = 1.0f - cursorPosition.y / (windowHeight * 0.5f);
	pickingRay.z = 1.0f;

	// Transform ray to world space

	XMVECTOR determinant;
	XMMATRIX matViewInverse = XMMatrixInverse(&determinant, XMLoadFloat4x4(pViewMatrix));

	XMVECTOR ray = XMVector3TransformCoord(XMLoadFloat3(&pickingRay), matViewInverse);

	XMFLOAT3 startPoint;	
	XMFLOAT3 endPoint;

	XMStoreFloat3(&startPoint, ray);
	XMStoreFloat3(&endPoint, ray);

	// Determine the point where the line intersects the grid plane
	XMStoreFloat3( pIntersection, XMPlaneIntersectLine( XMLoadFloat4( pPlane ), XMLoadFloat3(&startPoint), XMLoadFloat3(&endPoint) ) );
}

#endif // HELPERS_H