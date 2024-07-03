
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
using namespace CK::DD;

// ���ڸ� �׸��� �Լ�
void SoftRenderer::DrawGizmo2D()
{
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// �׸��� ����
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// ���� ���� ���
	Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// ���� �ϴܿ������� ���� �׸���
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// �׸��尡 ���۵Ǵ� ���ϴ� ��ǥ �� ���
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		r.DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		r.DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	r.DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	r.DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}

// ���� ������Ʈ ���


// ���� �� �ε��� ����ϴ� �Լ�
void SoftRenderer::LoadScene2D()
{
	// ���� �� �ε����� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();

}

// ���� ������ ������ ������ �����ϴ� ����
Vector2 currentPosition(100.f, 100.f);

// ���� ������ ����ϴ� �Լ�
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// ���� �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// ���� ������ ���� ����
	static float moveSpeed = 100.f;

	// 2024-07-03 �߰� start ====================================================================================
	// 1. GetNormalize �Լ��� ����Ͽ� �Է� ���͸� ����ȭ
	Vector2 inputVector = Vector2(input.GetAxis(InputAxis::XAxis), input.GetAxis(InputAxis::YAxis)).GetNormalize();
	// 2024-07-03 �߰� end ======================================================================================
	Vector2 deltaPosition = inputVector * moveSpeed * InDeltaSeconds;

	// ��ü�� ���� ���� ����
	currentPosition += deltaPosition;
}

// ������ ������ ����ϴ� �Լ�
void SoftRenderer::Render2D()
{
	// ������ �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// ��濡 ���� �׸���
	DrawGizmo2D();

	// 2024-07-03 �߰� start ====================================================================================
	
	// 2. ���׸���
	// 2-1. ���� �������� 50���� ����
	// 2-2. ���� �߽� ��ġ�� currentPosition���� ����
	// 2-3. ���� �������� �������� ���� �׸���
	// 2-4. ���� �߽� ��ġ�� �������� ���� �׸���

	static float radius = 50.f; // ������
	static std::vector<Vector2> circles; // ���� ��ǥ�� ������ ����

	if (circles.empty()) // ���� ��ǥ�� ��������� ���� ��ǥ�� ���
	{
		for (float x = -radius; x <= radius; ++x) 
		{
			for (float y = -radius; y <= radius; ++y)
			{
				Vector2 pointToTest = Vector2(x, y);
				float squaredLength = pointToTest.SizeSquared();
				if (squaredLength <= radius * radius)
				{
					circles.push_back(Vector2(x, y));
				}
			}
		}
	}

	for (auto const& v : circles)
	{
		r.DrawPoint(currentPosition + v, LinearColor::Red);
	}

	// ���� 
	// 1, GetNormalize �Լ��� ����Ͽ� ������ ũ�⸦ �׻� 1�� ����ȭ�Ѵ�. 
	// 2. �׸� ���� �������� 50���� �����Ѵ�.
	// 3. �������ϴ� ���� �����ϱ� ���� �ڷᱸ�� Bector�� �����ϰ� �̸� circle�� �����Ѵ� �̶� �ѹ� ������ ������ ��� ������ �� �ֵ��� ������ static���� �����Ѵ�.
	// 4. circles �ڷᱸ���� ����ִ� ��쿡�� ���� �����ϴ� ���� ä���ֵ��� ������ �����Ѵ�. 
	// 5. ������ ������ Ȱ���� ���� ���Ե� �簢�� ������ ����ϰ�, �� �ȿ� ���� ���͸� �����ϱ� ���� ũ�� 1������ ������ ����. 
	// 6. x,y ���� ������ ���͸� �����Ѵ�. 
	// 7. ������ ũ�⸦ ���� ��, �������� ������ ���� �� x^2 + y^2 ���� ����Ѵ�. 
	// 8. 7�� ���� ũ�⸦ ���ϱ� ���� ������ ���� �� �� ���Ѵ�. �̷��� ũ���� �������� �������� �������� ���ϴ� ����� �������� ������� �ʱ� ������ �� ������ ����� ����. 
	// 9. 8�� ����� ���̸� ���� �����ϴ� �ڷᱸ�� circles�� �ش� ���͸� �߰��Ѵ�.
	// 10. ���� �����ϴ� �ڷᱸ�� circles�� ���� ��� ���� ��ȸ�Ѵ�.
	// 11. �� ���� ��ġ�� ���� ���� ���� ���ϰ� ���������� ��´�. 
	// 2024-07-03 �߰� end ======================================================================================
	// ������ ������ ���� ����

}

// �޽ø� �׸��� �Լ�
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// �ﰢ���� �׸��� �Լ�
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}